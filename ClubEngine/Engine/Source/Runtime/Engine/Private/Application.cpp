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

#include <Engine/Input/FActionKeyMapping.h>
#include <Engine/Input/UInputComponent.h>

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

    assetContext.CreateEngineDefaults();

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

        assetContext.registry.FlushPendingDestroy();
    }

    for(auto it = systems.rbegin(); it != systems.rend(); ++it)
        (*it)->Shutdown();

    return exitCode;
}

void print(const FInputActionValue& value)
{
    (void)value;
    CE_LOG(Info, "Jump fired");
}

void Application::PostInit()
{
    TObjectHandle<UInputAction> jumpHandle = GetAssetContext().Create<UInputAction>(
        EInputActionValueType::Bool, TArray<UInputTrigger*>{}, TArray<UInputModifier*>{});
    UInputAction* jump = GetAssetContext().Resolve(jumpHandle);

    FActionKeyMapping jumpMapping{EKey::SpaceBar, jump, {}, {}};

    AActor* actor = GetSystem<WorldSystem>()->GetWorld().SpawnActor<AActor>(FTransform());
    UInputComponent* input = actor->GetLevel()->CreateComponent<UInputComponent>(actor);

    const FDelegateHandle handle = input->BindAction(jump, ETriggerEvent::Triggered, &print);

    CE_LOG(Info, "Mapping key: {}, bind handle valid: {}", ToString(jumpMapping.GetKey()), handle.IsValid());
}

void Application::PreInit()
{

}

}
