#pragma once
#include<glm/glm.hpp>

class Transform 
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform(glm::vec3 pos = glm::vec3(0.0f),
		glm::vec3 rot = glm::vec3(0.0f),
		glm::vec3 scl = glm::vec3(1.0f))
		: position(pos), rotation(rot), scale(scl) { }

	virtual ~Transform() = default;

	//GetForward
	//GetUp
	//GetRight
};
