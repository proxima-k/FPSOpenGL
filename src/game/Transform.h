#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

class Transform 
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 pos = glm::vec3(0.0f),
		glm::quat rot = glm::quat(0, 0, 0, 0),
		glm::vec3 scl = glm::vec3(1.0f))
		: position(pos), rotation(rot), scale(scl) { }

	virtual ~Transform() = default;

	glm::vec3 getForward() const 
	{
		float yaw = glm::radians(rotation.x); // Convert to radians
		float pitch = glm::radians(rotation.y); // Convert to radians

		glm::vec3 forward;
		forward.x = cos(yaw) * cos(pitch);
		forward.y = sin(pitch);
		forward.z = sin(yaw) * cos(pitch);

		return glm::normalize(forward);
	}
};
