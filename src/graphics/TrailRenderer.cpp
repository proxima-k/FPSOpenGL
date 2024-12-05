#include "TrailRenderer.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#include "Renderer.h"


TrailRenderer::TrailRenderer() { }

TrailRenderer::TrailRenderer(TrailMesh* mesh, Shader* shader, Camera* camera, glm::vec3 color)
{
	setMesh(mesh);
	setShader(shader);
	setTargetCamera(camera);
	meshColor = color;
}

void TrailRenderer::draw(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
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

	// View matrix
	glm::mat4 view;
	view = glm::lookAt(targetCamera->transform.position, targetCamera->transform.position + targetCamera->transform.getForward(), targetCamera->transform.getUp());

	// Projection matrix
	glm::mat4 projection;
	projection = targetCamera->getProjectionMatrix();
	//projection = glm::perspective(glm::radians(45.f), targetCamera->getResolutionRatio(), 0.1f, 100.f);

	unsigned int modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int colorLoc = glGetUniformLocation(shader->GetID(), "u_Color");
	GLCall(glUniform3fv(colorLoc, 1, glm::value_ptr(meshColor)));
	
	GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->getVerticesCount()));
}