#pragma once
#include<vector>
#include<glm/glm.hpp>

#include "Transform.h"
#include "../graphics/Mesh.h" 

enum class Collision_Channel
{
	None, 
	Player,
	Enemy,
	Projectile,
};

class Entity
{
public:
	Mesh* meshToDraw = nullptr;
	Entity(	glm::vec3 position = glm::vec3(0.0f), 
			glm::vec3 rotation = glm::vec3(0.0f), 
			glm::vec3 scale = glm::vec3(1.0f))
		: transform(position, rotation, scale) {	}
	virtual ~Entity() = default;

	Entity(glm::vec3 position, Mesh* mesh, Shader* shader, Camera* camera);

	virtual void update(float deltaTime);
	virtual void draw();

	Transform transform;

	bool destroyed = false;

	Collision_Channel collision_channel = Collision_Channel::None;

};