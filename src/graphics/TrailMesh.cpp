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
    std::vector<float> testVertices;
    // form a triangle
	
    testVertices.push_back(0.0f);
	testVertices.push_back(0.0f);
    testVertices.push_back(0.0f);

    testVertices.push_back(10.0f);
    testVertices.push_back(10.0f);
    testVertices.push_back(0.0f);

    testVertices.push_back(10.0f);
    testVertices.push_back(-10.0f);
    testVertices.push_back(0.0f);


	VAO = new VertexArray();
	VBO = new VertexBuffer(&testVertices[0], testVertices.size() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VAO->AddBuffer(*VBO, layout);

	verticesCount = testVertices.size();
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
        int determineValue = segment.yOrigin > segment.y ? 1 : -1;

        auto it = std::find(trailSegmentList.begin(), trailSegmentList.end(), segment);
        int posInList = std::distance(trailSegmentList.begin(), it) + 1;
        float normalizedPos = static_cast<float>(posInList) / trailSegmentList.size(); // Normalize position (0 to 1)

        // define the easeOutCirc function
        auto easeOutCirc = [](float x) -> float {
            return  1 - (1 - x) * (1 - x);
            };

        float easedValue = easeOutCirc(normalizedPos);
        float segmentY = segment.yOrigin + (width * easedValue * determineValue);

        verticesVector.push_back(segment.x);
        verticesVector.push_back(segmentY);
        verticesVector.push_back(segment.z);
    }

    VAO->Bind();
    VBO->Bind();
    glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(float), verticesVector.data(), GL_DYNAMIC_DRAW);
    verticesCount = verticesVector.size() / 3;
    //verticesCount = verticesVector.size();
	std::cout << "======================" << std::endl;
    std::cout << verticesVector.size() << std::endl;
    std::cout << verticesCount << std::endl;
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