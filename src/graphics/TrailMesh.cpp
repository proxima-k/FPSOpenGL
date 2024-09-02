#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "TrailMesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

TrailMesh::TrailMesh(const float* vertices, unsigned int size) : verticesCount(size)
{
	VAO = new VertexArray();
	VBO = new VertexBuffer(vertices, size);

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);
}

TrailMesh::TrailMesh(std::vector<float> vertices)
	: TrailMesh(vertices.data(), vertices.size() * sizeof(float)) { }

void TrailMesh::addVertice(const glm::vec3 newVertice)
{
	verticesVector.push_back(newVertice.x);
	verticesVector.push_back(newVertice.y);
	verticesVector.push_back(newVertice.z);

	if (verticesVector.size() > maxTrailPoints * 3)
	{
		verticesVector.erase(verticesVector.begin(), verticesVector.begin() + 3);
	}

	// Update VBO with new data
	VBO->Bind();
	glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(float), verticesVector.data(), GL_DYNAMIC_DRAW);
	verticesCount = verticesVector.size() / 3; // Each vertex has 3 floats (x, y, z)

}

void TrailMesh::Bind()
{
	VAO->Bind();
}

TrailMesh::~TrailMesh()
{
	delete VAO;
	delete VBO;

	VAO = NULL;
	VBO = NULL;

	shader = NULL;
	camera = NULL;
}