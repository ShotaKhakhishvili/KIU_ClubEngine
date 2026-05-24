#include <Core/Types/CoreTypes.h>

#include <Project/ActiveProject.h>

#include <Engine/World/UVoxelGrid.h>
#include <Engine/AssetLibrary.h>

#include <fstream>

namespace CE
{
    
UVoxelGrid::UVoxelGrid(uint16 chunkSizeX, uint16 chunkSizeY, uint16 chunkSizeZ)
    : chunkSizeX(chunkSizeX), chunkSizeY(chunkSizeY), chunkSizeZ(chunkSizeZ)
{
    CompileChunkGenShader();
}

void UVoxelGrid::CompileChunkGenShader()
{   
    std::size_t pathHash = std::hash<std::string>{}(GetActiveProject().rootPath.string());
    std::string id = GetActiveProject().name + "_" + std::to_string(pathHash);
    std::filesystem::path intermediateDir = std::filesystem::path(CE_ENGINE_ROOT) / "Intermediate" / id;

    std::filesystem::create_directories(intermediateDir / "Shaders");

    std::ostringstream oss;
    oss << "#define CHUNK_SIZE_X "      << chunkSizeX                           << "\n";
    oss << "#define CHUNK_SIZE_Y "      << chunkSizeY                           << "\n";
    oss << "#define CHUNK_SIZE_Z "      << chunkSizeZ                           << "\n";
    oss << "#define CHUNK_BLOCK_COUNT " << chunkSizeX * chunkSizeY * chunkSizeZ << "\n";
    
    oss << CE::FileIO::ReadFileWithIncludes(std::filesystem::path(CE_ENGINE_ROOT) / "Engine/Source/Runtime/Engine/Public/Engine/Shaders/Voxel/ChunkGenImpl.comp");
    
    const std::string shaderCode = oss.str();

    shader = NewObject<UShader>(RHI::ShaderDesc{ std::vector<RHI::ShaderSource>{RHI::ShaderSource{RHI::ShaderStage::Compute, oss.str()} } }, "ChunkGen.comp");

    std::ofstream file(intermediateDir / "Shaders/ChunkGen.comp");
    file << shaderCode;

	CE_LOG(Info, "Compiled ChunkGen shader to {}", (intermediateDir / "Shaders/ChunkGen.comp").string());
}

}