#include "OBB.h"

OBB::OBB::OBB(Transform* transform, glm::vec3 bounds)
{
	center = transform->position;
	localAxes[0] = transform->getRight();
	localAxes[1] = transform->getUp();
	localAxes[2] = transform->getForward();
	halfLengths = bounds / 2.0f;
}

bool OBB::isOverlapping(const OBB& obb1, const OBB& obb2) {
	// check the 15 axes if there's any separated axis
	for (int i = 0; i < 3; i++) {
		if (!overlapOnAxis(obb1, obb2, obb1.localAxes[i])) return false;
		if (!overlapOnAxis(obb1, obb2, obb2.localAxes[i])) return false;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			glm::vec3 crossProductAxis = glm::cross(obb1.localAxes[i], obb2.localAxes[j]);
			if (!overlapOnAxis(obb1, obb2, crossProductAxis)) return false;
		}
	}

	return true; // no separating axis found, which means the OBBs are overlapping
}

float OBB::projectOBB(const OBB& obb, const glm::vec3& axis) 
{
	return 
		obb.halfLengths.x * glm::abs(glm::dot(axis, obb.localAxes[0])) +
		obb.halfLengths.y * glm::abs(glm::dot(axis, obb.localAxes[1])) +
		obb.halfLengths.z * glm::abs(glm::dot(axis, obb.localAxes[2]));
}

bool OBB::overlapOnAxis(const OBB& obb1, const OBB& obb2, const glm::vec3& axis)
{
	float proj1 = projectOBB(obb1, axis);
	float proj2 = projectOBB(obb2, axis);
	float distance = glm::abs(glm::dot(axis, obb2.center - obb1.center));

	return distance <= (proj1 + proj2);
}
