#include <Engine/World/AVoxelTerrain.h>

#include "Asset/IAssetResolver.h"

namespace CE
{

IMPLEMENT_CLASS(AVoxelTerrain)

AVoxelTerrain::AVoxelTerrain()
{
    voxelGrid = CreateDefaultSubobject<UVoxelGrid>("VoxelGrid");
    rootComponent = voxelGrid;
}

}