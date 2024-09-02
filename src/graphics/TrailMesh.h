#pragma once

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "../game/Camera.h"

class TrailMesh
{
private:
	std::vector<float> verticesVector;
	unsigned int verticesCount;


	VertexArray* VAO;
	VertexBuffer* VBO;

	Shader* shader;
	Camera* camera;


public:
	TrailMesh(const float* vertices, unsigned int size);
	TrailMesh(std::vector<float> vertices);
	~TrailMesh();

	void addVertice(const glm::vec3 newVertice);

	int getVerticesCount() { return verticesCount; }

	void Bind();

	int maxTrailPoints = 100;
};

