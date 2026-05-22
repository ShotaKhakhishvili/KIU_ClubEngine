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
    std::size_t pathHash = std::hash<std::string>{}(GetActiveProject().rootPath.string());
    std::string id = GetActiveProject().name + "_" + std::to_string(pathHash);
    std::filesystem::path intermediateDir = std::filesystem::path(CE_ENGINE_ROOT) / "Intermediate" / id;

    std::filesystem::create_directories(intermediateDir / "Shaders");

    std::ofstream file(intermediateDir / "Shaders/ChunkGen.comp");
    file << "#define CHUNK_SIZE_X "      << chunkSizeX                           << "\n";
    file << "#define CHUNK_SIZE_Y "      << chunkSizeY                           << "\n";
    file << "#define CHUNK_SIZE_Z "      << chunkSizeZ                           << "\n";
    file << "#define CHUNK_BLOCK_COUNT " << chunkSizeX * chunkSizeY * chunkSizeZ << "\n";
    
    file << CE::FileIO::ReadFileWithIncludes(std::filesystem::path(CE_ENGINE_ROOT) / "Engine/Source/Runtime/Engine/Public/Engine/Shaders/Voxel/ChunkGenImpl.comp");
}

}