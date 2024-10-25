#include "XP.h"
#include "../Game.h"

XP::XP(glm::vec3 position, MeshRenderer meshrenderer) 
	: Entity(position, meshrenderer), trailRenderer()
{
	collision_channel = Collision_Channel::XP;

	transform.position = position;
	transform.scale = glm::vec3(0.02f);
	transform.rotation = glm::vec3(0.0f);

	meshRenderer = meshrenderer;

	glm::vec3 xpColor = glm::vec3(1.0f, 1.0f, 0.0f);
	meshRenderer.setColor(xpColor);

	Shader* trailShader = new Shader("res/shaders/mesh.shader");
	TrailMesh* trailMesh = new TrailMesh({});

	trailMesh->maxTrailPoints = 20;

	trailRenderer.setMesh(trailMesh);
	trailRenderer.setShader(trailShader);
	trailRenderer.setTargetCamera(game->camera);
	trailRenderer.setColor(meshRenderer.getColor());
}

void XP::update(float delta_time)
{
	transform.rotation *= glm::quat(glm::vec3(0.0f, 0.01f, 0.01f));

	glm::vec3 cameraPos = game->camera->transform.position;
	transform.position = glm::mix(transform.position, cameraPos, 5 * delta_time);

	if (glm::distance(lastPosition, transform.position) > 0.05f)
	{
		TrailMesh* trailMesh = trailRenderer.getMesh();

		glm::vec3 newPos = transform.position;

		trailMesh->addVertex(glm::vec3(newPos), .025f);

		lastPosition = transform.position;
	}

	trailRenderer.draw(transform.position, transform.rotation, glm::vec3(1.0f));
}