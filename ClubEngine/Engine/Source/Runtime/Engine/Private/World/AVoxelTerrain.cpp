#include <Engine/World/AVoxelTerrain.h>

#include "Asset/IAssetResolver.h"

namespace CE
{

AVoxelTerrain::AVoxelTerrain(uint16 chunkSizeX, uint16 chunkSizeY, uint16 chunkSizeZ)
    : AActor(FTransform())
{
    voxelGrid = CreateDefaultSubobject<UVoxelGrid>(chunkSizeX, chunkSizeY, chunkSizeZ);
    voxelGrid->SetParentComponent(GetRootComponent());
}

}