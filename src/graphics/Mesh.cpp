// Kent
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tinyobj/tiny_obj_loader.h>

#include <iostream>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(const float* vertices, unsigned int size) :
	vertices(vertices), verticesCount(size)
{
	// setup vertex array, vertex buffer
	//VAO = VertexArray();
	VAO = new VertexArray();
	VBO = new VertexBuffer(vertices, size);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	
	VAO->AddBuffer(*VBO, layout);
}

Mesh::~Mesh()
{
	delete VAO;
	delete VBO;

	VAO = NULL;
	VBO = NULL;

	shader = NULL;
	camera = NULL;
}

Mesh::Mesh(std::vector<float> vertices)
	: Mesh(vertices.data(), vertices.size() * sizeof(float)) { }


void Mesh::draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Camera& camera)
{
	VAO->Bind();
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
	view = glm::lookAt(camera.transform.position, camera.transform.position + camera.transform.getForward(), glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);

	unsigned int modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLCall(glDrawArrays(GL_LINE_LOOP, 0, verticesCount));
}

void Mesh::draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	draw(position, rotation, scale, *camera);
}

std::vector<float> Mesh::getMeshVerticesFromObjFile(const std::string& filePath)
{
	std::string inputFile = filePath;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./";

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputFile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader Error: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader Warning: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	// Creating a vector of floats filled with the model's vertex positions
	std::vector<float> vertices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			float vertexXPos = attrib.vertices[3 * index.vertex_index + 0];
			float vertexYPos = attrib.vertices[3 * index.vertex_index + 1];
			float vertexZPos = attrib.vertices[3 * index.vertex_index + 2];

			//std::cout << "X: " << vertexXPos << ", Y: " << vertexYPos << ", Z: " <<  vertexZPos << std::endl;

			vertices.push_back(vertexXPos);
			vertices.push_back(vertexYPos);
			vertices.push_back(vertexZPos);
		}
	}
	return vertices;
}
