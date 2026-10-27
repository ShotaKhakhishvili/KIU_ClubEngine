#include <Renderer/Scene/FScene.h>

#include "Asset/IAssetResolver.h"

namespace CE
{

FPrimitiveSceneProxyHandle FScene::AddProxy(const FPrimitiveSceneProxy& newProxy)
{
    return proxyPool.Add(newProxy);
}

bool FScene::RemoveProxy(const FPrimitiveSceneProxyHandle& proxyToRemove)
{
    return proxyPool.Remove(proxyToRemove);
}

bool FScene::UpdateTransform(FPrimitiveSceneProxyHandle handle, const FTransform& transform)
{
    FPrimitiveSceneProxy* proxy = proxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->localToWorld = FMatrix4x4F(transform.ToMatrix());
    return true;
}
bool FScene::UpdateStaticMesh(FPrimitiveSceneProxyHandle handle, UStaticMesh* staticMesh)
{
    FPrimitiveSceneProxy* proxy = proxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->staticMesh = staticMesh;
    return true;
}
bool FScene::SetVisible(FPrimitiveSceneProxyHandle handle, bool isVisible)
{
    FPrimitiveSceneProxy* proxy = proxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->isVisible = isVisible;

    return true;
}

}