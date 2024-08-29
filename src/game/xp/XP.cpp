#include "XP.h"
#include "../Game.h"

XP::XP(glm::vec3 position, MeshRenderer meshrenderer) 
	: Entity(position, meshrenderer)
{
	collision_channel = Collision_Channel::XP;

	transform.position = position;
	transform.scale = glm::vec3(0.02f);
	transform.rotation = glm::vec3(0.0f);

	meshRenderer = meshrenderer;

	glm::vec3 xpColor = glm::vec3(1.0f, 1.0f, 0.0f);
	meshRenderer.setColor(xpColor);
}

void XP::update(float delta_time)
{
	transform.rotation *= glm::quat(glm::vec3(0.0f, 0.01f, 0.01f));

	glm::vec3 cameraPos = game->camera->transform.position;
	transform.position = glm::mix(transform.position, cameraPos, 5 * delta_time);
}