#pragma once

#include <deque>
#include <vector>

#include "glm/glm.hpp"
#include "../graphics/MeshRenderer.h"
#include "../graphics/TrailMesh.h"

class Trail
{
public:
	Trail(glm::vec3 position, MeshRenderer* meshRenderer);

	void addPoint(glm::vec3 newPosition);
	void render();

	float width = 1.0f;

private:
	glm::vec3 position;
	MeshRenderer* meshRenderer;
	TrailMesh* mesh;
	
	std::deque<glm::vec3> trailPoints;
	int maxtrailPoints = 100;
};