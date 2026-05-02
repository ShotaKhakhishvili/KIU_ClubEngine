#include <AssetImport/ObjMeshImporter.h>

#include <iostream>

int main()
{
    CE::ObjImportSettings settings;
    settings.flipV = true;
    settings.triangulate = true;
    settings.scale = 1.0f;

    CE::ObjMeshImporter importer(settings);

    auto mesh = importer.Import("Assets/cube.obj");

    if (!mesh)
    {
        for (const auto& error : importer.GetErrors())
            std::cout << "Error: " << error << "\n";

        return -1;
    }

    for (const auto& warning : importer.GetWarnings())
        std::cout << "Warning: " << warning << "\n";

    std::cout << "Loaded OBJ\n";
    std::cout << "Vertices: " << mesh->vertices.size() << "\n";
    std::cout << "Indices: " << mesh->indices.size() << "\n";
    std::cout << "Sections: " << mesh->sections.size() << "\n";

    return 0;
}