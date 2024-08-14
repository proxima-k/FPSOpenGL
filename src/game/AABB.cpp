#include "AABB.h"

AABB AABB::from_position_size(Transform transform)
{
	return AABB(
		transform.position - transform.scale * 0.5f,
		transform.position + transform.scale * 0.5f
	);
}

bool aabb_overlap(AABB a, AABB b)
{
	return
		a.max.x > b.min.x && b.max.x > a.min.x &&
		a.max.y > b.min.y && b.max.y > a.min.y &&
		a.max.z > b.min.z && b.max.z > a.min.z;
}