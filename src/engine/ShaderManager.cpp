#include "ShaderManager.h"

ShaderManager* shaderManager = nullptr;
const std::filesystem::path filePath = "res/shaders/";

void ShaderManager::init()
{
    std::vector<std::string> loadStrings;
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string shaderName = entry.path().stem().string();
            loadStrings.push_back(shaderName);
        }
    }

    for (const std::string& shaderName : loadStrings) {
        loadShader(shaderName);
    }

    std::cout << "Shaders loaded!" << std::endl;
}

void ShaderManager::loadShader(std::string shaderName)
{
    std::filesystem::path fullFilePath = filePath / (shaderName + ".shader");

	Shader* newShader = new Shader(fullFilePath.string());
	shaders[shaderName] = newShader;
}
