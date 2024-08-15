#include "AABB.h"
#include <iostream>

AABB AABB::from_position_size(Transform transform)
{
	glm::vec3 min = transform.position - transform.scale * 0.5f;
	glm::vec3 max = transform.position + transform.scale * 0.5f;

	return AABB(min, max);
}

bool aabb_overlap(AABB a, AABB b)
{
    bool overlap_x = a.max.x >= b.min.x && b.max.x >= a.min.x;
    bool overlap_y = a.max.y >= b.min.y && b.max.y >= a.min.y;
    bool overlap_z = a.max.z >= b.min.z && b.max.z >= a.min.z;

    return overlap_x && overlap_y && overlap_z;
}
