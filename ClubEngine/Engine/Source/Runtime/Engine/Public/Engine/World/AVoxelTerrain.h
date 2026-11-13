#pragma once

#include <Core/ClubCore.h>

#include <Engine/World/AActor.h>
#include <Engine/World/UVoxelGrid.h>

namespace CE
{

class AVoxelTerrain : public AActor
{
    GENERATED_BODY(AVoxelTerrain, AActor)
    
public:
    AVoxelTerrain();

private:
    UVoxelGrid* voxelGrid;

    std::unordered_map<uint32, EVoxelChunkState> chunkStates;
};

}