#pragma once

#include <Core/ClubCore.h>

#include <unordered_map>

namespace CE
{
    
enum class RenderBackend
{
    OpenGL
};

struct FVoxelChunk
{
    std::unique_ptr<uint16[]> blockBuffer;
    uint32 blockCount;
    uint8 blockDataSize;
    uint32 blockDataMask;

    void Initialize(uint32 inBlockCount, uint32 inBlockDataSize);

    uint16 UnsafeGetBlockType(uint32 blockIndex);
    void UnsafeSetBlockType(uint32 blockIndex, uint16 newType);

    uint16 GetBlockType(uint32 blockIndex);
    void SetBlockType(uint32 blockIndex, uint16 newType);
};

struct FVoxelChunkFaceMap
{
    std::unordered_map<uint32, uint32[6]> faceHandles;
};

struct ChunkHash {
    size_t operator()(uint32 key) const
    {
        key = ((key >> 16) ^ key) * 0x45d9f3b;
        key = ((key >> 16) ^ key) * 0x45d9f3b;
        return (key >> 16) ^ key;
    }
};

enum class EVoxelChunkState
{
    None,
    Disk,
    VoxelGenPending,
    VoxelReadback,
    FaceGenPending,
    Ready
};

}