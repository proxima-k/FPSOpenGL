#include "WallGrid.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include "../graphics/VertexBufferLayout.h"

WallGrid::WallGrid(float cellLength, int xCellCount, int zCellCount, int yCellCount)
	: cellLength(cellLength), xCellCount(xCellCount), zCellCount(zCellCount), yCellCount(yCellCount)
{
	std::vector<float> vertices;

	for (int y = 1; y <= yCellCount; y++) {
		// 4 points which creates 4 lines
		// A - top left, B - top right, C - bottom right, D - bottom left
		// distance from the center to corner of an axis
		float centerToCornerX = xCellCount * cellLength; 
		float centerToCornerZ = zCellCount * cellLength;
		glm::vec3 pointA( centerToCornerX, y * cellLength,  centerToCornerZ);
		glm::vec3 pointB(-centerToCornerX, y * cellLength,  centerToCornerZ);
		glm::vec3 pointC(-centerToCornerX, y * cellLength, -centerToCornerZ);
		glm::vec3 pointD( centerToCornerX, y * cellLength, -centerToCornerZ);

		vertices.push_back(pointA.x);
		vertices.push_back(pointA.y);
		vertices.push_back(pointA.z);
		vertices.push_back(pointB.x);
		vertices.push_back(pointB.y);
		vertices.push_back(pointB.z);

		vertices.push_back(pointB.x);
		vertices.push_back(pointB.y);
		vertices.push_back(pointB.z);
		vertices.push_back(pointC.x);
		vertices.push_back(pointC.y);
		vertices.push_back(pointC.z);

		vertices.push_back(pointC.x);
		vertices.push_back(pointC.y);
		vertices.push_back(pointC.z);
		vertices.push_back(pointD.x);
		vertices.push_back(pointD.y);
		vertices.push_back(pointD.z);

		vertices.push_back(pointD.x);
		vertices.push_back(pointD.y);
		vertices.push_back(pointD.z);
		vertices.push_back(pointA.x);
		vertices.push_back(pointA.y);
		vertices.push_back(pointA.z);
	}

	for (int z = -zCellCount; z <= zCellCount; z++) {
		for (int x = -xCellCount; x <= xCellCount; x += xCellCount * 2) {
			vertices.push_back(x * cellLength);
			vertices.push_back(0);
			vertices.push_back(z * cellLength);

			vertices.push_back(x * cellLength);
			vertices.push_back(yCellCount * cellLength);
			vertices.push_back(z * cellLength);
		}
	}

	for (int x = -xCellCount + 1; x < xCellCount; x++) {
		for (int z = -zCellCount; z <= zCellCount; z += zCellCount * 2) {
			vertices.push_back(x * cellLength);
			vertices.push_back(0);
			vertices.push_back(z * cellLength);

			vertices.push_back(x * cellLength);
			vertices.push_back(yCellCount * cellLength);
			vertices.push_back(z * cellLength);
		}
	}

	verticesCount = vertices.size();

	VAO = new VertexArray();
	VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);
}

WallGrid::~WallGrid()
{
	delete VAO;
	delete VBO;

	VAO = NULL;
	VBO = NULL;

	camera = NULL;
	shader = NULL;
}

void WallGrid::draw()
{
	VAO->Bind();
	shader->Bind();

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.position);
	model *= glm::toMat4(transform.rotation);
	model = glm::scale(model, transform.scale);

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(camera->transform.position, camera->transform.position + camera->transform.getForward(), camera->transform.getUp());

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.f), camera->getResolutionRatio(), 0.1f, 100.f);

	//shader->SetFloat("u_displayHeight", -glm::cos(timer) + 1.0f);
	shader->SetFloat("u_displayHeight", 1.0f);

	shader->SetMat4("u_Model", model);
	shader->SetMat4("u_View", view);
	shader->SetMat4("u_Projection", projection);

	glm::vec3 bgColor(0.1f);
	shader->SetFloat3("u_bgColor", bgColor);
	shader->SetFloat("u_renderRadius", yCellCount * cellLength);
	shader->SetFloat3("u_playerPos", player->transform.position);


	GLCall(glDrawArrays(GL_LINES, 0, verticesCount));
}

void WallGrid::setShader(Shader* _shader)
{
	shader = _shader;
}

void WallGrid::setCamera(Camera* _camera)
{
	camera = _camera;
}

void WallGrid::setPlayer(Player* _player)
{
	player = _player;
}