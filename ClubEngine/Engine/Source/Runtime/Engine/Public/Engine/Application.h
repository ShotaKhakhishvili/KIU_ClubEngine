#pragma once

#include <Engine/AssetContext.h>
#include <Engine/ISystem.h>

#include <vector>

namespace CE
{

class Application {
public:
    Application();
    virtual ~Application();

    template<typename T,typename... Args>
    T& AddSystem(Args&&... args)
    {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *ptr;
        systems.emplace_back(std::move(ptr));
        return ref;
    }

    template<typename T> requires std::is_base_of_v<ISystem, T>
    T* GetSystem() const
    {
        for(auto& system : systems)
        {
            if(T* s = dynamic_cast<T*>(system.get()))
                return s;
        }

        return nullptr;
    }

    int Run();
    virtual int Update(float dt) = 0;

    AssetContext& GetAssetContext() noexcept;
    const AssetContext& GetAssetContext() const noexcept;

protected:
    virtual void PreInit();
	virtual void PostInit();
        
    std::vector<std::unique_ptr<ISystem>> systems;
private:
    bool running{false};
        
    AssetContext assetContext{};
};

}