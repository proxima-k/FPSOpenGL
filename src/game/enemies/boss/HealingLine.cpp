#include "HealingLine.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include "../../../graphics/VertexBufferLayout.h"

#include <ShaderManager.h>

HealingLine::HealingLine(glm::vec3 startPosition, glm::vec3 endPosition)
	: startPosition(startPosition), endPosition(endPosition)
{
	shader = shaderManager->getShader("healline");
	camera = Camera::mainCamera;

	verticesData.push_back(startPosition.x);
	verticesData.push_back(startPosition.y);
	verticesData.push_back(startPosition.z);

	verticesData.push_back(endPosition.x);
	verticesData.push_back(endPosition.y);
	verticesData.push_back(endPosition.z);

	verticesDataCount = verticesData.size();

	VAO = new VertexArray();
	VBO = new VertexBuffer(&verticesData[0], verticesDataCount * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);
}

void HealingLine::update(float deltaTime)
{
	timeElapsed += deltaTime;
}

void HealingLine::draw()
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

	shader->SetMat4("u_Model", model);
	shader->SetMat4("u_View", view);
	shader->SetMat4("u_Projection", projection);

	shader->SetFloat("u_timeElapsed", timeElapsed);
	shader->SetFloat3("u_startPos", startPosition);
	shader->SetFloat3("u_endPos", endPosition);

	GLCall(glDrawArrays(GL_LINES, 0, verticesDataCount));
}

void HealingLine::updateStartPosition(glm::vec3 newStartPosition)
{
	startPosition = newStartPosition;

	verticesData[0] = newStartPosition.x;
	verticesData[1] = newStartPosition.y;
	verticesData[2] = newStartPosition.z;

	VAO->Bind();
	VBO->updateData(&verticesData[0], verticesDataCount * sizeof(float));
}

void HealingLine::updateEndPosition(glm::vec3 newEndPosition)
{
	endPosition = newEndPosition;

	verticesData[3] = newEndPosition.x;
	verticesData[4] = newEndPosition.y;
	verticesData[5] = newEndPosition.z;

	VAO->Bind();
	VBO->updateData(&verticesData[0], verticesDataCount * sizeof(float));
}
