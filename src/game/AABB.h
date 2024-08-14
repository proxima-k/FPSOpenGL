#pragma once

#include<glm/glm.hpp>

#include "Transform.h"

struct AABB
{
	static AABB from_position_size(Transform transform);

	AABB(glm::vec3 in_min, glm::vec3 in_max) : min(in_min), max(in_max) { }

	glm::vec3 min;
	glm::vec3 max;
};

bool aabb_overlap(AABB a, AABB b);
