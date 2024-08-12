#pragma once

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class Mesh
{
private:
	const float* vertices;
	unsigned int verticesCount;
	
	VertexArray* VAO;
	VertexBuffer* VBO;

	// pointer to a Transform

public:
	// todo: add indices
	Mesh(std::vector<float> vertices);
	Mesh(const float* vertices, unsigned int size);
	~Mesh();

	void draw(Shader& shader);

	static std::vector<float> getMeshVerticesFromObjFile(const std::string& filePath);
};
