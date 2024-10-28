// Kent
#pragma once

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "../game/Camera.h"

/*
	This class essentially stores vertices data of a mesh and allocates a buffer in the GPU and stores the data there
*/
class Mesh
{
private:
	const float* vertices;
	unsigned int verticesCount;
	
	VertexArray* VAO;
	VertexBuffer* VBO;

public:
	// todo: add indices
	Mesh(const float* vertices, unsigned int size);
	Mesh(std::vector<float> vertices);
	~Mesh();

	void Bind();

	// draws a static mesh with constant values of view and model matrix
	//void draw(Shader& shader);	
	// draws a mesh with constant value of model matrix but dynamic view matrix
	//void draw(Shader& shader, Camera& camera);
	/*void draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Camera* camera);
	void draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void setShader(Shader* _shader) { shader = _shader; }
	void setCamera(Camera* _camera) { camera = _camera; }*/

	int getVerticesCount() { return verticesCount; }

	static std::vector<float> getMeshVerticesFromObjFile(const std::string& filePath);
};

