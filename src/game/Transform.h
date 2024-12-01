#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glm::vec3 getRight()   const { return glm::normalize(rotation * glm::vec3(1, 0, 0)); }
	glm::vec3 getUp()      const { return glm::normalize(rotation * glm::vec3(0, 1, 0)); }
	glm::vec3 getForward() const { return glm::normalize(rotation * glm::vec3(0, 0, 1)); }

	glm::mat4 getLocalTransformMatrix() const {
		// local transform matrix is the same as model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, position);
		model *= glm::toMat4(rotation);
		model = glm::scale(model, scale);
		return model;
	}

	glm::mat4 computeWorldTransform(const Transform& parentWorldTransform, const Transform& childTransform) {
		return parentWorldTransform.getLocalTransformMatrix() * childTransform.getLocalTransformMatrix();
	}

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

	void lookAt(glm::vec3 targetPosition, glm::vec3 worldUp = glm::vec3(0, 1, 0)) 
	{
		glm::vec3 forward = glm::normalize(targetPosition - position);
		glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
		glm::vec3 up = glm::cross(forward, right);

		// Create a rotation matrix from the basis vectors
		glm::mat3 rotationMatrix = glm::mat3(right, up, forward);

		// Convert rotation matrix to quaternion
		rotation = glm::quat_cast(rotationMatrix);
	}
};

// old code
/*
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
*/