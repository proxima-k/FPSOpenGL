#pragma once

#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/Shader.h"
#include "../game/Camera.h"
#include "../game/Player.h"

class WallGrid {
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
	glm::vec2 getCellPosInt(glm::vec3 position);
	void getCellCenter(int x, int y, glm::vec3& center);
	void getCellCenter(glm::vec3 position, glm::vec3& center);

	Transform transform;
};