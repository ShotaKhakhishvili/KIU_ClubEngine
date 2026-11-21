#include <Core/Config/ConfigSystem.h>
#include <Core/FileIO.h>
#include <Core/CommandLine.h>

#include <CoreUObject/AssetRegistry.h>

#include <AssetImport/Image.h>
#include <Asset/UShader.h>
#include <Asset/UMaterial.h>
#include <Asset/UStaticMesh.h>
#include <Asset/ImportAssetBuilder.h>

#include <Platform/FWindowConfig.h>
#include <Project/ActiveProject.h>

#include <Engine/Application.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/Systems/RenderSystem.h>
#include <Engine/Systems/WindowSystem.h>
#include <Engine/Systems/WorldSystem.h>
#include <Engine/FOfflineRenderSettings.h>

#include <filesystem>
#include <ranges>
#include <string>
#include <vector>
#include <chrono>
#include <format>

namespace CE
{

namespace
{
FOfflineRenderSettings ReadOfflineRenderSettings(const ConfigSystem& config)
{
    FOfflineRenderSettings s;
    s.enabled               = config.Get<bool> ("OfflineRender", "Enabled");
    s.forcedOpaqueAlpha     = config.Get<bool>  ("OfflineRender", "ForceOpaqueAlpha");
    s.fps                   = config.Get<float>("OfflineRender", "FPS");
    s.startFrame            = config.Get<uint32>  ("OfflineRender", "StartFrame");
    s.endFrame              = config.Get<uint32>  ("OfflineRender", "EndFrame");
    s.width                 = config.Get<uint32>  ("OfflineRender", "Width");
    s.height                = config.Get<uint32>  ("OfflineRender", "Height");

    const std::filesystem::path out = config.Get<std::string>("OfflineRender", "OutputDir");
    s.outputDir = out.is_relative() ? GetActiveProject().GetProjectPath() / out : out;

    return s;
}

FWindowConfig ReadWindowConfig(const ConfigSystem& config)
{

    FWindowConfig windowConfig;
    windowConfig.title  = config.Get<std::string>("Render", "Title");
    windowConfig.width  = config.Get<int>("Render", "Width");
    windowConfig.height = config.Get<int>("Render", "Height");
    windowConfig.vSync  = config.Get<bool>("Render", "VSync");
    windowConfig.visible  = config.Get<bool>("Render", "Visible");

    return windowConfig;
}
}

Application::Application()
{
    ConfigSystem config;
    config.LoadFile(GetActiveProject().GetEngineConfigPath() / "BaseRender.ini");
    config.LoadFile(GetActiveProject().GetConfigPath() / "DefaultRender.ini");
    config.AddCommandLine(CommandLine::GetArgs());

    offlineRenderSettings = ReadOfflineRenderSettings(config);
    FWindowConfig windowConfig = ReadWindowConfig(config);

    if (offlineRenderSettings.enabled)
    {
        windowConfig.visible = false;
        windowConfig.vSync   = false;
    }

    auto& windowSystem = AddSystem<WindowSystem>(windowConfig);
    AddSystem<InputSystem>(&windowSystem);
    auto& worldSystem = AddSystem<WorldSystem>();
    auto& renderSystem = AddSystem<RenderSystem>(&windowSystem, &worldSystem, ERenderBackend::OpenGL);

    if (offlineRenderSettings.enabled)
        renderSystem.SetFixedTargetSize(offlineRenderSettings.width, offlineRenderSettings.height);
}

bool Application::CaptureFrame(const RenderSystem& renderSystem, const uint32 frame) const
{
    TArray<uint8> pixels;
    if (!renderSystem.ReadSceneColor(pixels))
    {
        CE_LOG(Error, "OfflineRender: failed to read frame {}", frame);
        return false;
    }

    if (pixels.size() != static_cast<size_t>(offlineRenderSettings.width) * offlineRenderSettings.height * 4)
    {
        CE_LOG(Error, "OfflineRender: frame {} size mismatch", frame);
        return false;
    }

    if (offlineRenderSettings.forcedOpaqueAlpha)
    {
        for (size_t i = 3; i < pixels.size(); i += 4)
            pixels[i] = 255;
    }

    const auto path = offlineRenderSettings.outputDir / std::format("frame_{:04}.png", frame);
    if (!CE_STB::SavePNG(path, pixels.data(), offlineRenderSettings.width, offlineRenderSettings.height, 4))
    {
        CE_LOG(Error, "OfflineRender: failed to write {}", path.string());
        return false;
    }

    return true;
}

Application::~Application() = default;

void Application::CreateAssetDefaults()
{
    using std::filesystem::path;

    AssetRegistry& assets = GetAssetRegistry();
    const path enginePath = GetActiveProject().GetEnginePath();

    const path vertPath = enginePath / "Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.vert";
    const path fragPath = enginePath / "Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.frag";

    RHI::ShaderDesc shaderDesc;
    shaderDesc.stages = {
        { RHI::ShaderStage::Vertex,   FileIO::ReadFileWithIncludes(vertPath) },
        { RHI::ShaderStage::Fragment, FileIO::ReadFileWithIncludes(fragPath) }
    };

    const TObjectHandle<UShader> shaderHandle = assets.Create<UShader>(shaderDesc);

    if (UShader* shader = assets.Resolve(shaderHandle))
    {
        shader->SetName("DefaultShader");

        if (!shader->IsReady())
            shader->Load();
    }

    const TObjectHandle<UMaterial> materialHandle = assets.Create<UMaterial>();

    if (UMaterial* material = assets.Resolve(materialHandle))
    {
        material->SetName("DefaultMaterial");
        material->SetShader(shaderHandle);
    }

    UMaterial::SetDefault(materialHandle);

    const path meshPath = enginePath / "Engine/Source/Runtime/Engine/Public/Engine/Geometry/TestModel.glb";

    const TObjectHandle<UStaticMesh> meshHandle = ImportStaticMesh(meshPath);

    if (UStaticMesh* mesh = assets.Resolve(meshHandle))
        mesh->SetName("DebugStaticMesh");
    else
        CE_LOG(Warning, "Debug mesh import failed: {}", meshPath.string());

    UStaticMesh::SetDefault(meshHandle);
}

void Application::ReleaseAssetDefaults()
{
    UStaticMesh::SetDefault({});
    UMaterial::SetDefault({});
}

int Application::Run()
{
    if (offlineRenderSettings.enabled && !offlineRenderSettings.Validate())
        return 1;

    this->PreInit();

    this->running = true;
    size_t systemsInitialized = 0;

    for (const auto& s : systems)
    {
        if(!s->Init())
        {
            running = false;
            break;
        }
        systemsInitialized++;
    }

    if(!running)
    {
        size_t i = systemsInitialized;
        while(i-- > 0)
        {
            systems[i]->Shutdown();
        }
        return 1;
    }

    CreateAssetDefaults();

    this->PostInit();

    const int exitCode = offlineRenderSettings.enabled ? RunOfflineLoop() : RunInteractiveLoop();

    ReleaseAssetDefaults();

    for (const auto & system : std::ranges::reverse_view(systems))
        system->Shutdown();

    return exitCode;
}

int Application::RunInteractiveLoop()
{
    using Clock = std::chrono::steady_clock;
    auto previous = Clock::now();

    int exitCode = 0;

    while(this->running)
    {
        const auto now = Clock::now();
        const float dt = std::chrono::duration<float>(now - previous).count();
        previous = now;

        for (const auto& s : systems)
        {
            exitCode = s->Update(dt);
            if(exitCode != 0)
            {
                running = false;
                break;
            }
        }

        if(!running)
            break;

        exitCode = this->Update(dt);

        if(exitCode != 0)
        {
            running = false;
            break;
        }

        GetAssetRegistry().FlushPendingDestroy();
    }

    return exitCode;
}

int Application::RunOfflineLoop()
{
    int exitCode = 0;

    const float dt = offlineRenderSettings.GetDeltaTime();
    const uint32 frames = offlineRenderSettings.endFrame + 1;
    const uint32 startFrame = offlineRenderSettings.startFrame;

    const auto* renderSystem = GetSystem<RenderSystem>();

    std::error_code ec;
    std::filesystem::create_directories(offlineRenderSettings.outputDir, ec);
    if (ec)
    {
        CE_LOG(Error, "OfflineRender: can't create {}: {}", offlineRenderSettings.outputDir.string(), ec.message());
        return 1;
    }

    for(uint32 frame = 0; frame < frames && this->running; frame++)
    {
        for (const auto& s : systems)
        {
            exitCode = s->Update(dt);
            if(exitCode != 0)
            {
                running = false;
                break;
            }
        }

        if(!this->running)
            break;

        exitCode = this->Update(dt);

        if (frame >= startFrame && !CaptureFrame(*renderSystem, frame))
            return 1;

        if(exitCode != 0)
        {
            running = false;
            break;
        }

        GetAssetRegistry().FlushPendingDestroy();
    }

    return exitCode;
}

void Application::PostInit()
{

}

void Application::PreInit()
{

}

}
