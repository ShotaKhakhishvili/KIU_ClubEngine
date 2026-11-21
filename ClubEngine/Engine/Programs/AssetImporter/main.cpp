#include <AssetImport/GltfImporter.h>

#include <iostream>

int main(int argc, char** argv)
{
    const std::filesystem::path path = argc > 1 ? argv[1] : "Assets/cube.glb";

    CE::GltfImporter importer;

    const std::optional<CE::FSceneImportResult> scene = importer.Import(path);

    for (const auto& warning : importer.GetWarnings())
        std::cout << "Warning: " << warning << "\n";

    if (!scene)
    {
        for (const auto& error : importer.GetErrors())
            std::cout << "Error: " << error << "\n";

        return -1;
    }

    size_t vertices = 0;
    size_t indices = 0;
    size_t sections = 0;

    for (const CE::FMeshImportResult& mesh : scene->meshes)
    {
        vertices += mesh.vertices.size();
        indices += mesh.indices.size();
        sections += mesh.sections.size();
    }

    std::cout << "Loaded glTF: " << path.string() << "\n";
    std::cout << "Nodes: " << scene->nodes.size() << " (roots: " << scene->rootNodes.size() << ")\n";
    std::cout << "Meshes: " << scene->meshes.size() << "\n";
    std::cout << "Materials: " << scene->materials.size() << "\n";
    std::cout << "Textures: " << scene->textures.size() << "\n";
    std::cout << "Vertices: " << vertices << "\n";
    std::cout << "Indices: " << indices << "\n";
    std::cout << "Sections: " << sections << "\n";

    return 0;
}