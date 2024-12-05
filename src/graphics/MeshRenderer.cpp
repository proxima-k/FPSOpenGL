#include "MeshRenderer.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#include "Renderer.h"


MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader, Camera* camera, glm::vec3 color)
{
	setMesh(mesh);
	setShader(shader);
	setTargetCamera(camera);
	meshColor = color;
}

void MeshRenderer::draw(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model *= glm::toMat4(rotation);
	model = glm::scale(model, scale);

	draw(model);
}

void MeshRenderer::draw(Transform& transform) {
	draw(transform.position, transform.rotation, transform.scale);
}

void MeshRenderer::draw(glm::mat4 modelMatrix)
{
	if (mesh == nullptr) {
		std::cout << "[MeshRenderer] Error: Missing mesh in mesh renderer" << std::endl;
		return;
	}
	if (shader == nullptr) {
		std::cout << "[MeshRenderer] Error: Missing shadeer in mesh renderer" << std::endl;
		return;
	}
	if (targetCamera == nullptr) {
		std::cout << "[MeshRenderer] Error: Missing camera in mesh renderer" << std::endl;
		return;
	}

	mesh->Bind();
	shader->Bind();

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(targetCamera->transform.position, targetCamera->transform.position + targetCamera->transform.getForward(), targetCamera->transform.getUp());

	// Projection matrix
	glm::mat4 projection;
	projection = targetCamera->getProjectionMatrix();
	//projection = glm::perspective(glm::radians(45.f), targetCamera->getResolutionRatio(), 0.1f, 100.f);

	shader->SetMat4("u_Model", modelMatrix);
	shader->SetMat4("u_View", view);
	shader->SetMat4("u_Projection", projection);

	shader->SetFloat3("u_Color", meshColor);

	GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->getVerticesCount()));
}
