#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "../game/Camera.h"

/*
	A MeshRenderer stores a Mesh reference that it should render.
	It also stores a shader, a camera, which means all of them can point to a single mesh/shader/camera instance.
	This means memory can be saved.
 */
class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Shader* shader, Camera* camera, glm::vec3 color = glm::vec3(1.f));

	void draw(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	
	void setMesh(Mesh* mesh) { this->mesh = mesh; }
	void setShader(Shader* shader) { this->shader = shader; }
	void setTargetCamera(Camera* camera) { this->targetCamera = camera; }
	void setColor(glm::vec3 color) { this->meshColor = color; }

	glm::vec3 getColor() { return meshColor; }

private:
	// mesh data to be used for rendering
	Mesh* mesh = nullptr;
	
	// shader to be used to render the mesh
	Shader* shader = nullptr;

	// which lens should the mesh be rendered
	// this means the mesh will be rendered in the lens of the targetCamera
	Camera* targetCamera = nullptr;
	
	glm::vec3 meshColor;
};

