#include <Project/ActiveProject.h>

#include <Engine/World/UVoxelGrid.h>

#include <fstream>

namespace CE
{
    
UVoxelGrid::UVoxelGrid(uint16 chunkSizeX, uint16 chunkSizeY, uint16 chunkSizeZ)
    : chunkSizeX(chunkSizeX), chunkSizeY(chunkSizeY), chunkSizeZ(chunkSizeZ)
{
    WriteVoxelConfig();
}

void UVoxelGrid::WriteVoxelConfig() const
{
    std::filesystem::path intermediateDir = GetActiveProject().rootPath / "Intermediate";

    std::filesystem::create_directories(intermediateDir);

    std::ofstream file(intermediateDir / "VoxelConfig.glsl");
    file << "#define CHUNK_SIZE_X "      << chunkSizeX      << "\n";
    file << "#define CHUNK_SIZE_Y "      << chunkSizeY      << "\n";
    file << "#define CHUNK_SIZE_Z "      << chunkSizeZ      << "\n";
    file << "#define CHUNK_BLOCK_COUNT " << chunkSizeX * chunkSizeY * chunkSizeZ  << "\n";
}

}