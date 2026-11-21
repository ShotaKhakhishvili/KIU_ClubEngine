#include <Renderer/Scene/FScene.h>

namespace CE
{

FPrimitiveSceneProxyHandle FScene::AddPrimitive(const FPrimitiveSceneProxy& newProxy)
{
    return primitiveProxyPool.Add(newProxy);
}

bool FScene::RemovePrimitive(const FPrimitiveSceneProxyHandle& proxyToRemove)
{
    return primitiveProxyPool.Remove(proxyToRemove);
}

FLightSceneProxyHandle FScene::AddLight(const FLightSceneProxy& newProxy)
{
    return lightProxyPool.Add(newProxy);
}

bool FScene::RemoveLight(const FLightSceneProxyHandle& proxyToRemove)
{
    return lightProxyPool.Remove(proxyToRemove);
}

bool FScene::UpdateLightTransform(FLightSceneProxyHandle handle, const FTransform& transform)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->localToWorld = FMatrix4x4F(transform.ToMatrix());
    return true;
}

bool FScene::UpdateLightParameters(FLightSceneProxyHandle handle, const FLightParameters& lightParameters)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters = lightParameters;
    return true;
}

bool FScene::UpdateLightColor(FLightSceneProxyHandle handle, const FVectorF& color)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters.color = color;
    return true;
}

bool FScene::UpdateLightIntensity(FLightSceneProxyHandle handle, float intensity)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters.intensity = intensity;
    return true;
}

bool FScene::UpdateLightAttenuationRadius(FLightSceneProxyHandle handle, float attenuationRadius)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters.attenuationRadius = attenuationRadius;
    return true;
}

bool FScene::UpdateLightInnerConeAngle(FLightSceneProxyHandle handle, float innerConeAngle)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters.innerConeAngle = innerConeAngle;
    return true;
}

bool FScene::UpdateLightOuterConeAngle(FLightSceneProxyHandle handle, float outerConeAngle)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->lightParameters.outerConeAngle = outerConeAngle;
    return true;
}

bool FScene::UpdateLightVisibility(FLightSceneProxyHandle handle, bool isVisible)
{
    FLightSceneProxy* proxy = lightProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->isVisible = isVisible;
    return true;
}

bool FScene::UpdatePrimitiveTransform(FPrimitiveSceneProxyHandle handle, const FTransform& transform)
{
    FPrimitiveSceneProxy* proxy = primitiveProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->localToWorld = FMatrix4x4F(transform.ToMatrix());
    return true;
}

bool FScene::UpdatePrimitiveStaticMesh(FPrimitiveSceneProxyHandle handle, TObjectHandle<UStaticMesh> staticMesh)
{
    FPrimitiveSceneProxy* proxy = primitiveProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->staticMesh = staticMesh;
    return true;
}

bool FScene::UpdatePrimitiveVisibility(FPrimitiveSceneProxyHandle handle, bool isVisible)
{
    FPrimitiveSceneProxy* proxy = primitiveProxyPool.Resolve(handle);

    if(proxy == nullptr)
        return false;

    proxy->isVisible = isVisible;

    return true;
}

}