#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

TextureManager* textureManager = nullptr;

void TextureManager::init() {
    std::vector<std::string> loadStrings;
    std::string folderPath = std::filesystem::current_path().string() + filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string textureName = entry.path().stem().string();
            loadStrings.push_back(textureName);
            std::cout << "Found texture: " << textureName << std::endl; // log each found texture
        }
    }

    for (const std::string& textureName : loadStrings) {
        std::cout << textureName << std::endl;
        loadTexture(textureName);
    }
}

void TextureManager::loadTexture(std::string textureName) {
    std::string _filePath = filePath + textureName + ".png";
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true); // flip image if needed
    unsigned char* data = stbi_load(_filePath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << " Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // upload the texture data
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // free image data after uploading to GPU

    textures[textureName] = texture;
    std::cout << "Loaded texture: " << textureName << " with ID: " << texture << std::endl;
}