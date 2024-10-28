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

	// Creating a vector of floats filled with the model's vertex positions
    // Creating a vector of floats filled with the model's vertex positions and normals
    std::vector<float> verticesAndNormals;

    bool hasNormals = !attrib.normals.empty();

    // Storage for vertex normals if we need to calculate them
    std::vector<glm::vec3> vertexNormals(attrib.vertices.size() / 3, glm::vec3(0.0f));

    // Iterate over shapes and calculate normals if necessary
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
            // get 3 indices that forms a triangle
            tinyobj::index_t idx0 = shape.mesh.indices[i];
            tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
            tinyobj::index_t idx2 = shape.mesh.indices[i + 2];

            glm::vec3 v0(attrib.vertices[3 * idx0.vertex_index + 0], attrib.vertices[3 * idx0.vertex_index + 1], attrib.vertices[3 * idx0.vertex_index + 2]);
            glm::vec3 v1(attrib.vertices[3 * idx1.vertex_index + 0], attrib.vertices[3 * idx1.vertex_index + 1], attrib.vertices[3 * idx1.vertex_index + 2]);
            glm::vec3 v2(attrib.vertices[3 * idx2.vertex_index + 0], attrib.vertices[3 * idx2.vertex_index + 1], attrib.vertices[3 * idx2.vertex_index + 2]);

            glm::vec3 normal0, normal1, normal2;

            if (!hasNormals) {
                // Calculate the face normal
                glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                // Accumulate the face normal for each vertex
                vertexNormals[idx0.vertex_index] += faceNormal;
                vertexNormals[idx1.vertex_index] += faceNormal;
                vertexNormals[idx2.vertex_index] += faceNormal;

                normal0 = faceNormal;
                normal1 = faceNormal;
                normal2 = faceNormal;
            }
            else {
                // If normals are present in the OBJ file, use them directly
                normal0 = glm::vec3(attrib.normals[3 * idx0.normal_index + 0],
                    attrib.normals[3 * idx0.normal_index + 1],
                    attrib.normals[3 * idx0.normal_index + 2]);

                normal1 = glm::vec3(attrib.normals[3 * idx1.normal_index + 0],
                    attrib.normals[3 * idx1.normal_index + 1],
                    attrib.normals[3 * idx1.normal_index + 2]);

                normal2 = glm::vec3(attrib.normals[3 * idx2.normal_index + 0],
                    attrib.normals[3 * idx2.normal_index + 1],
                    attrib.normals[3 * idx2.normal_index + 2]);
            }

            // vertex 1 position
            verticesAndNormals.push_back(v0.x);
            verticesAndNormals.push_back(v0.y);
            verticesAndNormals.push_back(v0.z);
            // vertex 1 normal
            verticesAndNormals.push_back(normal0.x);
            verticesAndNormals.push_back(normal0.y);
            verticesAndNormals.push_back(normal0.z);
          
            // vertex 2 position
            verticesAndNormals.push_back(v1.x);
            verticesAndNormals.push_back(v1.y);
            verticesAndNormals.push_back(v1.z);
            // vertex 2 normal
            verticesAndNormals.push_back(normal1.x);
            verticesAndNormals.push_back(normal1.y);
            verticesAndNormals.push_back(normal1.z);

            // vertex 3 position
            verticesAndNormals.push_back(v2.x);
            verticesAndNormals.push_back(v2.y);
            verticesAndNormals.push_back(v2.z);
            // vertex 3 normal
            verticesAndNormals.push_back(normal2.x);
            verticesAndNormals.push_back(normal2.y);
            verticesAndNormals.push_back(normal2.z);
          
        }
    }

    return verticesAndNormals;
}
