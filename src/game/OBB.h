#pragma once

#include "Transform.h"
#include <glm/vec3.hpp>

namespace OBB {
	// OBB stands for oriented bounding box
	// which compared to AABB, it accounts for the box's rotation on top of the box's size
	struct OBB {
		OBB(Transform* transform, glm::vec3 bounds);

		glm::vec3 center;			// center of OBB
		glm::vec3 localAxes[3];		// local XYZ axes
		glm::vec3 halfLengths;		// half lengths on each axis
	};

	// main function to use 
	bool isOverlapping(const OBB& obb1, const OBB& obb2);

	float projectOBB(const OBB& obb, const glm::vec3& axis);
	bool overlapOnAxis(const OBB& obb1, const OBB& obb2, const glm::vec3& axis);
}