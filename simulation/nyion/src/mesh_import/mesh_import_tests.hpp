#include "mesh_import.hpp"

TEST(mesh_import,mesh_import_1){
    objl::Loader Loader;

    bool return_value = Loader.LoadFile("10mm_cube.obj");

    if(!return_value){
        abort("Failed to load file");
    }

    objl::Mesh curMesh = Loader.LoadedMeshes[i];

    file << "Mesh " << i << ": " << curMesh.MeshName << "\n";
    file << "Vertices:\n";

    for (int j = 0; j < curMesh.Vertices.size(); j++){

    }
}
