#pragma once

#include <vector>

#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/Shader.h"
#include "../game/Camera.h"
#include "../game/Player.h"

class Grid
{
private:
	unsigned int verticesCount;
	float cellWidth, cellHeight;
	int cellsPerSideOfAxis;

	VertexArray* VAO;
	VertexBuffer* VBO;

	Shader* shader;
	Camera* camera;

	Player* player;

public:
	Grid(float cellWidth, float cellHeight, int cellsPerSideOfAxis);
	~Grid();

	void update();
	void draw();

	void setShader(Shader* shader);
	void setCamera(Camera* camera);

	void setPlayer(Player* player);
	glm::vec2 getCellCoords(glm::vec3 position);
	glm::vec2 getCellCenter(int x, int y);
	glm::vec2 getCellCenter(glm::vec3 position);

	Transform transform;
};

extern Grid* floorGrid;