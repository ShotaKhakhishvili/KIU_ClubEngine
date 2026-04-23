#pragma once

#include <Engine/AssetContext.h>

namespace CE {
    class Application {
    public:
        Application();
        virtual ~Application();

        virtual int Run() = 0;

        AssetContext& GetAssetContext() noexcept;
        const AssetContext& GetAssetContext() const noexcept;

    private:
        AssetContext assetContext{};
    };
}