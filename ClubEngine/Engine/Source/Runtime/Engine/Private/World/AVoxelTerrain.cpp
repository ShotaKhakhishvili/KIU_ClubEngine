#include <Engine/World/AVoxelTerrain.h>

namespace CE
{

AVoxelTerrain::AVoxelTerrain(uint16 chunkSizeX, uint16 chunkSizeY, uint16 chunkSizeZ)
    : voxelGrid(new UVoxelGrid(chunkSizeX, chunkSizeY, chunkSizeZ))
{

}

}