#pragma once

#include <glew/glew.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "../graphics/Shader.h"

class ShaderManager
{
public:
	std::unordered_map<std::string, Shader*> shaders;

	void init();
	void loadShader(std::string shaderName);

	Shader* getShader(std::string fileName)
	{
		if (shaders.find(fileName) != shaders.end()) {
			return shaders[fileName];
		}
		std::cout << "no shader found" << std::endl;

		return 0;
	}
};

extern ShaderManager* shaderManager;