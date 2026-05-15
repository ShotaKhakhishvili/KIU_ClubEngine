#include <Core/ClubCore.h>
#include <Core/Config/ConfigSystem.h>

#include <Platform/WindowConfig.h>

#include <Project/ActiveProject.h>

#include <Engine/Application.h>
#include <Engine/Systems/RenderSystem.h>
#include <Engine/Systems/WindowSystem.h>

#include <string>
#include <chrono>

namespace CE
{

Application::Application()
{
    ConfigSystem config;
    config.LoadFile(GetActiveProject().GetConfigPath() / "DefaultRender.ini");

    WindowConfig windowConfig;
    windowConfig.title  = config.Get<std::string>("Render", "Title", "ClubEngine");
    windowConfig.width  = config.Get<int>("Render", "Width", 1280);
    windowConfig.height = config.Get<int>("Render", "Height", 720);
    windowConfig.vSync  = config.Get<bool>("Render", "VSync", true);

    auto& windowSystem = AddSystem<CE::WindowSystem>(windowConfig);
    AddSystem<CE::RenderSystem>(&windowSystem, CE::RenderBackend::OpenGL);
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
    this->running = true;

    for(auto& s : systems)
    {
        if(!s->Initialize())
            return 1;
    }

    using Clock = std::chrono::steady_clock;
    auto previous = Clock::now();

    while(this->running)
    {
        const auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - previous).count();
        previous = now;

        for(auto& s : systems)
        {
            const int code = s->Update(dt);
            if(code != 0)
                return code;
        }

        const int code = this->Update(dt);

        if(code != 0)
            return code;
    }

    for(auto it = systems.begin(); it != systems.end(); ++it)
        (*it)->Shutdown();

    return 0;
}

}
