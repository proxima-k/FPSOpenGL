#include "Grid.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#include "../graphics/VertexBufferLayout.h"

Grid::Grid(float cellWidth, float cellHeight, int cellsPerSideOfAxis)
	: cellWidth(cellWidth), cellHeight(cellHeight)
{
	std::vector<float> vertices;

	// creating vertical lines
	for (int i = -cellsPerSideOfAxis; i <= cellsPerSideOfAxis; i++) {
		vertices.push_back(i * cellWidth);
		vertices.push_back(0);
		vertices.push_back(-cellsPerSideOfAxis * cellHeight);

		vertices.push_back(i * cellWidth);
		vertices.push_back(0);
		vertices.push_back(cellsPerSideOfAxis * cellHeight);

		//vertices.push_back(glm::vec3(i * cellWidth, 0, -cellsPerSideOfAxis * cellHeight));
		//vertices.push_back(glm::vec3(i * cellWidth, 0,  cellsPerSideOfAxis * cellHeight));
	}

	// creating horizontal lines
	for (int j = -cellsPerSideOfAxis; j <= cellsPerSideOfAxis; j++) {
		vertices.push_back(-cellsPerSideOfAxis * cellWidth);
		vertices.push_back(0);
		vertices.push_back(j * cellHeight);

		vertices.push_back( cellsPerSideOfAxis * cellWidth);
		vertices.push_back(0);
		vertices.push_back(j * cellHeight);

		//vertices.push_back(glm::vec3(-cellsPerSideOfAxis * cellWidth, 0, j * cellWidth));
		//vertices.push_back(glm::vec3( cellsPerSideOfAxis * cellWidth, 0, j * cellWidth));
	}
	verticesCount = vertices.size();

	VAO = new VertexArray();
	VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);
}

Grid::~Grid()
{
	delete VAO;
	delete VBO;

	VAO = NULL;
	VBO = NULL;

	camera = NULL;
	shader = NULL;
}

void Grid::update()
{
	float difference;
	int steps;

	difference= player->transform.position.x - transform.position.x;
	steps = difference / cellWidth;
	transform.position.x += steps;
	
	difference = player->transform.position.z - transform.position.z;
	steps = difference / cellHeight;
	transform.position.z += steps;
}

void Grid::draw()
{
	VAO->Bind();
	shader->Bind();

	glm::vec3 position(0);
	glm::vec3 rotation(0);
	glm::vec3 scale(1);

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(camera->transform.position, camera->transform.position + camera->transform.getForward(), glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);

	unsigned int modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 bgColor(0.1f);
	unsigned int bgColorLoc = glGetUniformLocation(shader->GetID(), "u_bgColor");
	GLCall(glUniform3fv(bgColorLoc, 1, glm::value_ptr(bgColor)));

	GLCall(glDrawArrays(GL_LINES, 0, verticesCount));
}

void Grid::setShader(Shader* _shader)
{
	shader = _shader;
}

void Grid::setCamera(Camera* _camera)
{
	camera = _camera;
}

void Grid::setPlayer(Player* _player)
{
	player = _player;
}
