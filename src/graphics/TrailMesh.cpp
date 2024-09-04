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

void TrailMesh::addVertex(const glm::vec3 newVertice, float width)
{
    // store all variables
    float x = newVertice.x;
    float y = newVertice.y;
    float z = newVertice.z;
    float yOrigin = newVertice.y;

    // define new segments accounting for offset
    TrailSegment segmentTop (x, y + width / 2, z, y);
    TrailSegment segmentBottom (x, y - width / 2, z, y);

    // add vertex with positive and negative offsets
    trailSegmentList.push_back(segmentTop);
    trailSegmentList.push_back(segmentBottom);

    // clear the outdated trail points
    if (trailSegmentList.size() > maxTrailPoints * 2)
    {
        trailSegmentList.erase(trailSegmentList.begin(), trailSegmentList.begin() + 2);
    }

    // temporary vertex list for the buffer to read
    std::vector<float> verticesVector;

    // fill the vertex list with the points from the segments
    for (const auto& segment : trailSegmentList)
    {
        verticesVector.push_back(segment.x);
        verticesVector.push_back(segment.y);
        verticesVector.push_back(segment.z);
    }

    VBO->Bind();
    glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(float), verticesVector.data(), GL_DYNAMIC_DRAW);
    verticesCount = verticesVector.size() / 3;
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