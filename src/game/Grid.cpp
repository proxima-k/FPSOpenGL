#include "Grid.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#include "../graphics/VertexBufferLayout.h"

Grid::Grid(float cellWidth, float cellHeight, int cellsPerSideOfAxis)
	: cellWidth(cellWidth), cellHeight(cellHeight), cellsPerSideOfAxis(cellsPerSideOfAxis)
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

	/*std::cout << "Player Pos";
	std::cout << player->transform.position.x << " " << player->transform.position.y << " " << player->transform.position.z << std::endl;
	std::cout << "Grid Pos";
	std::cout << transform.position.x << " " << transform.position.y << " " << transform.position.z << std::endl;*/
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

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.position);
	model *= glm::toMat4(transform.rotation);
	/*model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));*/

	model = glm::scale(model, transform.scale);

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(camera->transform.position, camera->transform.position + camera->transform.getForward(), camera->transform.getUp());

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.f), camera->getResolutionRatio(), 0.1f, 100.f);

	shader->SetMat4("u_Model", model);
	shader->SetMat4("u_View", view);
	shader->SetMat4("u_Projection", projection);
	/*unsigned int modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/

	glm::vec3 bgColor(0.1f);
	/*unsigned int bgColorLoc = glGetUniformLocation(shader->GetID(), "u_bgColor");
	GLCall(glUniform3fv(bgColorLoc, 1, glm::value_ptr(bgColor)));
	unsigned int renderRadiusLoc = glGetUniformLocation(shader->GetID(), "u_renderRadius");
	glUniform1f(renderRadiusLoc, cellWidth * (cellsPerSideOfAxis - 1));
	unsigned int gridWorldPosLoc = glGetUniformLocation(shader->GetID(), "u_gridWorldPos");
	GLCall(glUniform3fv(gridWorldPosLoc, 1, glm::value_ptr(transform.position)));*/
	
	shader->SetFloat3("u_bgColor", bgColor);
	shader->SetFloat("u_renderRadius", cellWidth* (cellsPerSideOfAxis - 1));
	shader->SetFloat3("u_gridWorldPos", transform.position);

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

glm::vec2 Grid::getCellPosInt(glm::vec3 position) 
{
	int x = position.x / cellWidth;
	int y = position.z / cellHeight;

	return glm::vec2(x, y);
}

void Grid::getCellCenter(int x, int y, glm::vec3& center)
{
	center.x = x * cellWidth + cellWidth / 2;
	center.y = 0;
	center.z = y * cellHeight + cellHeight / 2;

}

void Grid::getCellCenter(glm::vec3 position, glm::vec3& center)
{
}
