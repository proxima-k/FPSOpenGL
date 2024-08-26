#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

class Transform 
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 pos = glm::vec3(0.0f),
		glm::quat rot = glm::quat(1, 0, 0, 0),
		glm::vec3 scl = glm::vec3(1.0f))
		: position(pos), rotation(rot), scale(scl) { }

	virtual ~Transform() = default;

	glm::vec3 getForward() const {
		return glm::normalize(rotation * glm::vec3(0, 0, 1));

		//float yaw = glm::radians(rotation.y); // convert to radians
		//float pitch = glm::radians(rotation.x); // convert to radians

		//glm::vec3 forward;
		//forward.x = cos(yaw) * cos(pitch);
		//forward.y = sin(pitch);
		//forward.z = sin(yaw) * cos(pitch);

		//return glm::normalize(forward);
	}
	glm::vec3 getRight() const { return glm::normalize(rotation * glm::vec3(1, 0, 0)); }
	glm::vec3 getUp()    const { return glm::normalize(rotation * glm::vec3(0, 1, 0)); }

	glm::vec3 getRandomPointInRadius(int minDist, int maxDist) const
	{
		float angle = std::rand() % 360;
		float distance = minDist + std::rand() % maxDist;

		glm::vec3 point;
		point.x = distance * cos(glm::radians(angle));
		point.y = 0.0f;
		point.z = distance * sin(glm::radians(angle));

		return point;
	}

	float getVectorMagnitude(glm::vec3 vector)
	{
		float beforeSqrt = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
		return glm::sqrt(beforeSqrt);
	}
};
