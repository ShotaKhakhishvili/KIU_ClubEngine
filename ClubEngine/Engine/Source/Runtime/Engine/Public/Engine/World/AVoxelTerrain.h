#pragma once

#include <Engine/World/AActor.h>
#include <Engine/World/UVoxelGrid.h>

namespace CE
{

class AVoxelTerrain : public AActor
{
    GENERATED_BODY(AVoxelTerrain, AActor)
    
public:
    AVoxelTerrain(uint16 chunkSizeX, uint16 chunkSizeY, uint16 chunkSizeZ);

private:
    UVoxelGrid* voxelGrid;
    std::unordered_map<uint32, EVoxelChunkState> chunkStates;
};

}