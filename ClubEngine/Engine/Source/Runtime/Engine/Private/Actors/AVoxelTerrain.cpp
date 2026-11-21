#include <Engine/Actors/AVoxelTerrain.h>

namespace CE
{

IMPLEMENT_CLASS(AVoxelTerrain)

AVoxelTerrain::AVoxelTerrain()
{
    voxelGrid = CreateDefaultSubobject<UVoxelGrid>("VoxelGrid");
    rootComponent = voxelGrid;
}

}