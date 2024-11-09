#pragma once

#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/Shader.h"
#include "../game/Camera.h"
#include "../game/Player.h"

class WallGrid {
private:
	unsigned int verticesCount;
	float cellLength;
	int xCellCount, yCellCount, zCellCount;

	VertexArray* VAO;
	VertexBuffer* VBO;

	Shader* shader;
	Camera* camera;

	Player* player;

public:
	WallGrid(float cellLength, int xCellCount, int zCellCount, int yCellCount);
	~WallGrid();

	void update() {}
	void draw();

	void setShader(Shader* shader);
	void setCamera(Camera* camera);
	void setPlayer(Player* player);

	Transform transform;


	//glm::vec2 getBounds()
};