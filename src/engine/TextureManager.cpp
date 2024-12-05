#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

TextureManager* textureManager = nullptr;

const std::filesystem::path filePath = "res/sprites/";

void TextureManager::init() {
    std::vector<std::string> loadStrings;
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string textureName = entry.path().stem().string();
            loadStrings.push_back(textureName);

            //std::cout << "Texture Loaded :: " << textureName << std::endl;
        }
    }

    for (const std::string& textureName : loadStrings) {
        loadTexture(textureName);
    }

    std::cout << "Textures loaded!" << std::endl;
}

void TextureManager::loadTexture(std::string textureName) {
    std::filesystem::path fullFilePath = filePath / (textureName + ".png");
    int width, height, channels;

    unsigned char* data = stbi_load(fullFilePath.string().c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << fullFilePath << " Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    textures[textureName] = textureID;
}
