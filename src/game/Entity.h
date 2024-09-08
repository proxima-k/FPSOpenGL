#pragma once
#include<vector>
#include<glm/glm.hpp>

#include "Transform.h"
#include "../graphics/MeshRenderer.h" 
#include "../graphics/TrailRenderer.h"

enum class Collision_Channel
{
	None, 
	Player,
	Enemy,
	Projectile,
	XP,
};

class Entity
{
public:

	Entity(	glm::vec3 position = glm::vec3(0.0f), 
			glm::quat rotation = glm::quat(0, 0, 0, 0),
			glm::vec3 scale = glm::vec3(1.0f))
		: transform(position, rotation, scale) {	}
	virtual ~Entity() = default;

	Entity(glm::vec3 position, MeshRenderer meshRenderer);

	virtual void update(float deltaTime);
	virtual void draw();

	void destroy() { destroyed = true; }

	Transform transform;
	MeshRenderer meshRenderer;

	Collision_Channel collision_channel = Collision_Channel::None;

	bool destroyed = false;
};