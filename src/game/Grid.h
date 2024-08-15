#pragma once

#include <vector>

#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/Shader.h"
#include "../game/Camera.h"

class Grid
{
private:
	unsigned int verticesCount;

	VertexArray* VAO;
	VertexBuffer* VBO;

	Shader* shader;
	Camera* camera;

public:
	// width, height
	// create vertices for each line

	Grid(float cellWidth, float cellHeight, int cellsPerSideOfAxis);
	~Grid();

	void draw();

	void setShader(Shader* shader);
	void setCamera(Camera* camera);
};

