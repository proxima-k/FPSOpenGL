#include "MeshManager.h"

MeshManager* meshManager = nullptr;
const std::filesystem::path filePath = "res/models/";

void MeshManager::init()
{
    std::vector<std::string> loadStrings;
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string meshName = entry.path().stem().string();
            loadStrings.push_back(meshName);
        }
    }

    for (const std::string& meshName : loadStrings) {
        loadMesh(meshName);
    }

    std::cout << "Meshes loaded!" << std::endl;

    // print out all keys
	/*for (auto const& x : meshes)
	{
		std::cout << "Mesh: " << x.first << std::endl;
	}*/
}

void MeshManager::loadMesh(std::string meshName)
{
    std::filesystem::path fullFilePath = filePath / (meshName + ".obj");

    std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile(fullFilePath.string());
	Mesh* newMesh = new Mesh(vertices);
	meshes[meshName] = newMesh;
}