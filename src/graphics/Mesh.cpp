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
	layout.Push<float>(3);
	
	VAO->AddBuffer(*VBO, layout);
}

Mesh::Mesh(std::vector<float> vertices)
	: Mesh(vertices.data(), vertices.size() * sizeof(float)) { }

Mesh::~Mesh()
{
	delete VAO;
	delete VBO;

	VAO = NULL;
	VBO = NULL;

	shader = NULL;
	camera = NULL;
}

void Mesh::Bind()
{
	VAO->Bind();
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

	for (size_t i = 0; i < attrib.normals.size() / 3; i++) {
		std::cout << "Normal " << i << ": "
			<< attrib.normals[3 * i + 0] << ", "
			<< attrib.normals[3 * i + 1] << ", "
			<< attrib.normals[3 * i + 2] << std::endl;
	}


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

			if (index.normal_index >= 0) {
				float normalX = attrib.normals[3 * index.normal_index + 0];
				float normalY = attrib.normals[3 * index.normal_index + 1];
				float normalZ = attrib.normals[3 * index.normal_index + 2];

				// Push normals to the vertices vector
				vertices.push_back(normalX);
				vertices.push_back(normalY);
				vertices.push_back(normalZ);
				std::cout << "test" << std::endl;
			}
			else {
				// Handle case where normals are not present (e.g., use a default normal)
				vertices.push_back(0.0f); // Default normal X
				vertices.push_back(0.0f); // Default normal Y
				vertices.push_back(0.0f); // Default normal Z
			}
		}
	}
	return vertices;
}
