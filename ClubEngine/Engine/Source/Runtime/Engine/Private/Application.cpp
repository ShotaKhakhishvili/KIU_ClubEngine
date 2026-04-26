#include <Engine/Application.h>

#include <chrono>

namespace CE
{
    Application::Application() = default;
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
