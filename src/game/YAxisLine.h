#pragma once

#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Transform.h"

class YAxisLine {
public:
	YAxisLine();
	~YAxisLine();

	void init(Camera* camera, Player* player);
	void draw();

	Transform transform;

private:
	VertexArray* VAO = nullptr;
	VertexBuffer* VBO = nullptr;

	Shader* shader = nullptr;
	Camera* camera = nullptr;
	Player* player = nullptr;

	unsigned int verticesCount = 0;
};