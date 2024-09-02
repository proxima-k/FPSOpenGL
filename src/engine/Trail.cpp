#include "../engine/Trail.h"

Trail::Trail(glm::vec3 position, MeshRenderer* meshRenderer) 
	: position(position), meshRenderer(meshRenderer)
{
	mesh = new TrailMesh(std::vector<float>());
	trailPoints.push_back(position);
}

void Trail::addPoint(glm::vec3 newPosition)
{
	position = newPosition;
	trailPoints.push_back(newPosition);

	if (trailPoints.size() > maxtrailPoints)
	{
		trailPoints.pop_front();
	}

	std::vector<float> vertices;
}