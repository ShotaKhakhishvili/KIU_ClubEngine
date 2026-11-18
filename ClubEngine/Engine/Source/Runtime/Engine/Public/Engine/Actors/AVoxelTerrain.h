#pragma once

#include <Core/ClubCore.h>

#include <Engine/Actors/AActor.h>
#include <Engine/Components/UVoxelGrid.h>

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