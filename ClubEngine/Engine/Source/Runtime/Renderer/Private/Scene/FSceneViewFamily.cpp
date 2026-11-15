#include <Renderer/Scene/FScene.h>
#include <Renderer/FFrameData.h>
#include <Renderer/Scene/FSceneViewFamily.h>

namespace CE
{


FSceneViewFamily::FSceneViewFamily(FScene* scene, const FRenderTarget* renderTarget, TArray<FSceneView>&& views)
    : scene(scene), renderTarget(renderTarget), views(std::move(views))
{
}

const FScene* FSceneViewFamily::GetScene()                  const   { return scene;             }
const FRenderTarget* FSceneViewFamily::GetRenderTarget()    const   { return renderTarget;      }
const TArray<FSceneView>& FSceneViewFamily::GetViews()      const   { return views;             }
bool FSceneViewFamily::ShouldClearTarget()                  const   { return shouldClearTarget; }

}