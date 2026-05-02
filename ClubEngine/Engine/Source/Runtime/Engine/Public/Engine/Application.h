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

    int Run();
    virtual int Update(float dt) = 0;

    AssetContext& GetAssetContext() noexcept;
    const AssetContext& GetAssetContext() const noexcept;

private:
    std::vector<std::unique_ptr<ISystem>> systems;
    bool running{false};
        
    AssetContext assetContext{};
};

}