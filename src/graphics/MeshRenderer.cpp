#include "MeshRenderer.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Renderer.h"


MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader, Camera* camera)
{
	setMesh(mesh);
	setShader(shader);
	setTargetCamera(camera);
}

void MeshRenderer::draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
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

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(targetCamera->transform.position, targetCamera->transform.position + targetCamera->transform.getForward(), glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.f), targetCamera->getResolutionRatio(), 0.1f, 100.f);

	unsigned int modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->getVerticesCount()));
}
