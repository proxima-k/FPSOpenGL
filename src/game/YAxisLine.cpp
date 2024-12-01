#include "YAxisLine.h"

#include <ShaderManager.h>

#include "../graphics/Renderer.h"
#include "../graphics/VertexBufferLayout.h"

YAxisLine::YAxisLine()
{
	float length = 100;
	std::vector<float> vertices;

	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);

	vertices.push_back(0);
	vertices.push_back(length);
	vertices.push_back(0);

	verticesCount = vertices.size();

	VAO = new VertexArray();
	VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);
}

YAxisLine::~YAxisLine()
{
	delete VAO;
	delete VBO;

	VAO = nullptr;
	VBO = nullptr;

	camera = nullptr;
	shader = nullptr;
	player = nullptr;
}

void YAxisLine::init(Camera* _camera, Player* _player)
{
	shader = shaderManager->getShader("yaxis");
	camera = _camera;
	player = _player;
}

void YAxisLine::draw()
{
	if (glm::distance2(player->transform.position, camera->transform.position) > 16 * 16) return;

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

	glm::vec3 axisColor(0.5f, 0.99f, 0.f);
	shader->SetFloat3("u_axisColor", axisColor);

	GLCall(glDrawArrays(GL_LINES, 0, verticesCount));
}
