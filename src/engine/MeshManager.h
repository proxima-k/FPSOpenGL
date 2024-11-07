#pragma once

#include <glew/glew.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "../graphics/Mesh.h"

class MeshManager
{
public:
	std::unordered_map<std::string, Mesh*> meshes;

	void init();
	void loadMesh(std::string meshName);

	Mesh* getMesh(std::string fileName)
	{
		if (meshes.find(fileName) != meshes.end()) {
			return meshes[fileName];
		}
		std::cout << "no mesh found" << std::endl;

		return 0;
	}
};

extern MeshManager* meshManager;