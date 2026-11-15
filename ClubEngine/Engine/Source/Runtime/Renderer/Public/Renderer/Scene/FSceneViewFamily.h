#pragma once

#include <Core/Containers/TArray.h>

#include <Renderer/Scene/FSceneView.h>

namespace CE
{

class FScene;
class FRenderTarget;

class FSceneViewFamily
{
public:
    FSceneViewFamily(FScene* scene, const FRenderTarget* renderTarget, TArray<FSceneView>&& views);

    const FScene* GetScene() const;
    const FRenderTarget* GetRenderTarget() const;
    const TArray<FSceneView>& GetViews() const;
    bool ShouldClearTarget() const;


private:
    FScene* scene;
    const FRenderTarget* renderTarget;

    TArray<FSceneView> views;
    bool shouldClearTarget = true;
};

}
