#pragma once

#include <glm/vec3.hpp>
#include "../../../graphics/VertexArray.h"
#include "../../../graphics/VertexBuffer.h"
#include "../../../graphics/Shader.h"
#include "../../../game/Camera.h"


class HealingLine {
public:
	HealingLine(glm::vec3 startPosition, glm::vec3 endPosition);
	~HealingLine() { delete VAO; delete VBO; }

	void update(float deltaTime);
	void draw();

	void updateStartPosition(glm::vec3 newStartPosition);
	void updateEndPosition(glm::vec3 newEndPosition);

	void setShader(Shader* _shader) { shader = _shader; }
	void setCamera(Camera* _camera) { camera = _camera; }

	void destroy() { destroyed = true; }

	Transform transform;
	bool destroyed = false;

private:
	float timeElapsed = 0.f;

	glm::vec3 startPosition;
	glm::vec3 endPosition;

	std::vector<float> verticesData;
	unsigned int verticesDataCount;

	VertexArray* VAO;
	VertexBuffer* VBO;

	Shader* shader;
	Camera* camera;
};