#pragma once

#include <Core/Containers/TPool.h>
#include <Renderer/Scene/FPrimitiveSceneProxy.h>

namespace CE
{

using FPrimitiveSceneProxyHandle = TPoolHandle<FPrimitiveSceneProxy>;

class FScene
{
public:
    FPrimitiveSceneProxyHandle AddProxy (const FPrimitiveSceneProxy& newProxy);
    bool RemoveProxy                    (const FPrimitiveSceneProxyHandle& proxyToRemove);

    bool UpdateTransform                (FPrimitiveSceneProxyHandle handle, const FTransform& transform);
    bool UpdateStaticMesh               (FPrimitiveSceneProxyHandle handle, TObjectHandle<UStaticMesh> staticMesh);
    bool SetVisible                     (FPrimitiveSceneProxyHandle handle, bool isVisible);

    template<typename F>
    void ForEachPrimitive(F&& func) const
    {
        proxyPool.ForEach(std::forward<F>(func));
    }

private:
    TPool<FPrimitiveSceneProxy> proxyPool;
};

}