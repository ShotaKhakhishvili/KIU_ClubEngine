#pragma once

#include <Core/Containers/TPool.h>

#include <Renderer/Scene/FLightSceneProxy.h>
#include <Renderer/Scene/FPrimitiveSceneProxy.h>

namespace CE
{

using FPrimitiveSceneProxyHandle = TPoolHandle<FPrimitiveSceneProxy>;
using FLightSceneProxyHandle = TPoolHandle<FLightSceneProxy>;

class FScene
{
public:


    FLightSceneProxyHandle AddLight         (const FLightSceneProxy& newProxy);
    bool RemoveLight                        (const FLightSceneProxyHandle& proxyToRemove);
    bool UpdateLightTransform               (FLightSceneProxyHandle handle, const FTransform& transform);
    bool UpdateLightParameters              (FLightSceneProxyHandle handle, const FLightParameters& lightParameters);
    bool UpdateLightColor                   (FLightSceneProxyHandle handle, const FVectorF& color);
    bool UpdateLightIntensity               (FLightSceneProxyHandle handle, float intensity);
    bool UpdateLightAttenuationRadius       (FLightSceneProxyHandle handle, float attenuationRadius);
    bool UpdateLightInnerConeAngle          (FLightSceneProxyHandle handle, float innerConeAngle);
    bool UpdateLightOuterConeAngle          (FLightSceneProxyHandle handle, float outerConeAngle);
    bool UpdateLightVisibility              (FLightSceneProxyHandle handle, bool isVisible);

    FPrimitiveSceneProxyHandle AddPrimitive (const FPrimitiveSceneProxy& newProxy);
    bool RemovePrimitive                    (const FPrimitiveSceneProxyHandle& proxyToRemove);
    bool UpdatePrimitiveTransform           (FPrimitiveSceneProxyHandle handle, const FTransform& transform);
    bool UpdatePrimitiveStaticMesh          (FPrimitiveSceneProxyHandle handle, TObjectHandle<UStaticMesh> staticMesh);
    bool UpdatePrimitiveVisibility          (FPrimitiveSceneProxyHandle handle, bool isVisible);

    template<typename F>
    void ForEachPrimitive(F&& func) const;

    template<typename F>
    void ForEachLight(F&& func) const;

private:
    TPool<FPrimitiveSceneProxy> primitiveProxyPool;
    TPool<FLightSceneProxy> lightProxyPool;
};

}

#include "FScene.inl"