#pragma once
#include<vector>
#include<glm/glm.hpp>
#include<string>

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

	Entity(glm::vec3 position = glm::vec3(0.0f),
		glm::quat rotation = glm::quat(1, 0, 0, 0),
		glm::vec3 scale = glm::vec3(1.0f));
	~Entity() { delete meshRenderer; }
	virtual void initMeshRenderer() {}

	virtual void update(float deltaTime);
	virtual void draw();

	void destroy() { destroyed = true; }

	std::string entityName = "Name";

	Transform transform;
	MeshRenderer* meshRenderer;

	Collision_Channel collision_channel = Collision_Channel::None;

	bool destroyed = false;
};