#include <Core/ClubCore.h>
#include <Core/Config/ConfigSystem.h>

#include <Platform/FWindowConfig.h>

#include <Project/ActiveProject.h>

#include <Asset/IAssetResolver.h>
#include <Asset/FSubMesh.h>
#include <Asset/UStaticMesh.h>

#include <AssetImport/ObjMeshImporter.h>

#include <RenderCore/RHI/IRHI.h>

#include <Renderer/Scene/FScene.h>

#include <Engine/Application.h>
#include <Engine/AssetContext.h>
#include <Engine/AssetLibrary.h>
#include <Engine/Systems/RenderSystem.h>
#include <Engine/Systems/WindowSystem.h>
#include <Engine/Systems/WorldSystem.h>

#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

namespace CE
{

Application::Application()
{
    ConfigSystem config;
    config.LoadFile(GetActiveProject().GetConfigPath() / "DefaultRender.ini");

    FWindowConfig windowConfig;
    windowConfig.title  = config.Get<std::string>("Render", "Title", "ClubEngine");
    windowConfig.width  = config.Get<int>("Render", "Width", 1280);
    windowConfig.height = config.Get<int>("Render", "Height", 720);
    windowConfig.vSync  = config.Get<bool>("Render", "VSync", true);

    auto& windowSystem = AddSystem<WindowSystem>(windowConfig);
    auto& worldSystem = AddSystem<WorldSystem>();
    AddSystem<RenderSystem>(&windowSystem, &worldSystem, ERenderBackend::OpenGL);
}

Application::~Application() = default;

AssetContext& Application::GetAssetContext() noexcept
{
    return assetContext;
}

const AssetContext& Application::GetAssetContext() const noexcept
{
    return assetContext;
}
    
int Application::Run()
{
    this->PreInit();

    this->running = true;
    size_t systemsInitialized = 0;

    for(auto& s : systems)
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

	this->PostInit();

    using Clock = std::chrono::steady_clock;
    auto previous = Clock::now();

    int exitCode = 0;

    while(this->running)
    {
        const auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - previous).count();
        previous = now;

        for(auto& s : systems)
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
    }

    for(auto it = systems.rbegin(); it != systems.rend(); ++it)
        (*it)->Shutdown();

    return exitCode;
}

void Application::PostInit()
{
    using std::filesystem::path;

    const path vertPath = GetActiveProject().GetEnginePath() / path("Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.vert");
    const path fragPath = GetActiveProject().GetEnginePath() / path("Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.frag");

    RHI::ShaderDesc shaderDesc;
    shaderDesc.stages = {
        { RHI::ShaderStage::Vertex,   FileIO::ReadFileWithIncludes(vertPath) },
        { RHI::ShaderStage::Fragment, FileIO::ReadFileWithIncludes(fragPath) }
    };

    auto shaderHandle = assetContext.Create<UShader>(shaderDesc, "DebugShader");
    UShader* shader = assetContext.Resolve(shaderHandle);
    if (shader && !shader->IsReady())
        shader->Load();

    auto materialHandle = assetContext.Create<UMaterial>(shaderHandle);
    UMaterial* material = assetContext.Resolve(materialHandle);
    if (material)
    {
        material->SetVec3("u_LightPos", 5.0f, 10.0f, 20.0f);
        material->SetBool("u_UseTexture", false);
    }

    // MVP sword sits around x:0-10, z:8-14, so pull camera back
    const FMatrix4x4F model = FMatrix4x4F::Identity();
    const FMatrix4x4F view = FMatrix4x4F::LookAt({ 0.0f, -45.0f, 25.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    const FMatrix4x4F projection = FMatrix4x4F::Perspective(0.9f, 16.0f / 9.0f, 0.1f, 100.0f);

    if (shader)
    {
        shader->SetMat4("u_Model", model);
        shader->SetMat4("u_View", view);
        shader->SetMat4("u_Projection", projection);
    }

    const path objPath = GetActiveProject().GetEnginePath() / "Engine/Source/Runtime/Engine/Public/Engine/Geometry/sting.obj";
    ObjMeshImporter importer;
    auto importRes = importer.Import(objPath);
    if (!importRes.has_value())
    {
        CE_LOG(Warning, "Debug mesh import failed: {}", objPath.string());
        return;
    }

    const FMeshImportResult& meshRes = importRes.value();

    RHI::BufferDesc vbd{};
    vbd.type = RHI::BufferType::Vertex;
    vbd.usage = RHI::BufferUsage::StaticDraw;
    vbd.size = static_cast<uint32_t>(meshRes.vertices.size() * sizeof(FVertex));
    auto vbHandle = RHI::Get().CreateBuffer(vbd, meshRes.vertices.data());

    RHI::BufferDesc ibd{};
    ibd.type = RHI::BufferType::Index;
    ibd.usage = RHI::BufferUsage::StaticDraw;
    ibd.size = static_cast<uint32_t>(meshRes.indices.size() * sizeof(uint32_t));
    auto ibHandle = RHI::Get().CreateBuffer(ibd, meshRes.indices.data());

    RHI::VertexAttributeDesc a0{};
    a0.location = 0; a0.componentCount = 3; a0.dataType = RHI::ShaderDataType::Float;
    a0.stride = static_cast<uint32_t>(sizeof(FVertex));
    a0.offset = static_cast<uint32_t>(offsetof(FVertex, coord));

    RHI::VertexAttributeDesc a1 = a0; a1.location = 1; a1.offset = static_cast<uint32_t>(offsetof(FVertex, normal));
    RHI::VertexAttributeDesc a2 = a0; a2.location = 2; a2.offset = static_cast<uint32_t>(offsetof(FVertex, color));
    RHI::VertexAttributeDesc a3 = a0; a3.location = 3; a3.componentCount = 2; a3.offset = static_cast<uint32_t>(offsetof(FVertex, texUV));

    RHI::VertexArrayDesc vaDesc;
    vaDesc.attributes = { a0, a1, a2, a3 };
    auto vaoHandle = RHI::Get().CreateVertexArray(vbHandle, ibHandle, vaDesc);

    FSubMesh subMesh{};
    subMesh.vertexArray = vaoHandle;
    subMesh.drawDesc.primitiveType = RHI::PrimitiveType::Triangles;
    subMesh.drawDesc.indexType = RHI::IndexType::UInt32;
    subMesh.drawDesc.indexCount = static_cast<uint32_t>(meshRes.indices.size());
    subMesh.drawDesc.indexOffset = 0;
    subMesh.materialIndex = 0;

    auto meshHandle = assetContext.Create<UStaticMesh>(std::string("DebugMesh"));
    UStaticMesh* staticMesh = assetContext.Resolve(meshHandle);

    if (staticMesh)
    {
        staticMesh->AddMaterial(materialHandle);
        staticMesh->AddSubMesh(subMesh);
        
        FPrimitiveSceneProxy proxy = {};
        proxy.staticMesh = meshHandle;

        GetSystem<WorldSystem>()->GetWorld().GetScene().AddProxy(proxy);
    }
}

void Application::PreInit()
{

}

}
