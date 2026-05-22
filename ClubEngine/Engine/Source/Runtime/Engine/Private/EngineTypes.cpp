#include <Engine/EngineTypes.h>

namespace CE
{

void FVoxelChunk::Initialize(uint32 inBlockCount, uint32 inBlockDataSize)
{
    blockCount = inBlockCount;
    blockDataSize = inBlockDataSize;
    blockDataMask = (1<<blockDataSize) - 1;

    blockBuffer = std::make_unique<uint16[]>((blockCount * blockDataSize + 15) / 16);
}

uint16 FVoxelChunk::UnsafeGetBlockType(uint32 blockIndex)
{
    return blockBuffer[blockIndex];
}

void FVoxelChunk::UnsafeSetBlockType(uint32 blockIndex, uint16 newType)
{
    blockBuffer[blockIndex] = newType;
}


uint16 FVoxelChunk::GetBlockType(uint32 blockIndex)
{        
    if(blockCount <= blockIndex)
    {
        CE_LOG(Error, "Invalid Block Index For Vertex Chunk. Asked For Index:{}, Accessible Block Count:{}", blockIndex, blockCount);
        return 0;
    }

    uint32 bitOffset = blockIndex * blockDataSize;
    uint32 byteOffset = bitOffset / 16;
    uint32 bitShift = bitOffset % 16;

    uint32* raw = (uint32*)(blockBuffer.get() + byteOffset);
    return (*raw>>bitShift) & blockDataMask; 
}

void FVoxelChunk::SetBlockType(uint32 blockIndex, uint16 newType)
{
    if(blockCount <= blockIndex)
    {
        CE_LOG(Error, "Invalid Block Index For Vertex Chunk. Asked For Index:{}, Accessible Block Count:{}", blockIndex, blockCount);
        return;
    }

    uint32 bitOffset = blockIndex * blockDataSize;
    uint32 byteOffset = bitOffset / 16;
    uint32 bitShift = bitOffset % 16;
    uint32 maskShift = blockDataMask << bitShift;

    uint32* raw = (uint32*)(blockBuffer.get() + byteOffset);
    *raw = (*raw & ~maskShift) | ((newType << bitShift) & maskShift);
}

}