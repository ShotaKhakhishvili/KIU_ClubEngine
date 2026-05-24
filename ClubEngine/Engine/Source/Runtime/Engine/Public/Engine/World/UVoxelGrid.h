#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UShader.h>

#include <Engine/EngineTypes.h>

#include <Engine/World/USceneComponent.h>

#include <unordered_map>

namespace CE
{

class UVoxelGrid : public USceneComponent
{
    GENERATED_BODY(UVoxelGrid, USceneComponent)

public:
    UVoxelGrid(uint16 chunkSizeX = 16, uint16 chunkSizeY = 16, uint16 chunkSizeZ = 256);

private:
    void CompileChunkGenShader();

    uint16 chunkSizeX;
    uint16 chunkSizeY;
    uint16 chunkSizeZ;

    TObjectHandle<UShader> shader;

    std::unordered_map<uint32, FVoxelChunk, ChunkHash> chunks;
    std::unordered_map<uint32, FVoxelChunkFaceMap, ChunkHash> chunkFaceMaps;
    std::unordered_map<uint8, std::vector<uint32>, ChunkHash> freeFacesPerShader;
};

}