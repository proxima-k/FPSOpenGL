#include <iostream>

#include "CubeEnemy.h"
#include "../Game.h"

#include <MeshManager.h>
#include <ShaderManager.h>

CubeEnemy::CubeEnemy(glm::vec3 position)
	: Enemy(position), physicsBody(), trailRenderer()
{
	Shader* trailShader = shaderManager->getShader("trail");
	TrailMesh* trailMesh = new TrailMesh({});

	trailMesh->maxTrailPoints = 25;

	trailRenderer.setMesh(trailMesh);
	trailRenderer.setShader(trailShader);
	trailRenderer.setTargetCamera(game->camera);
	//trailRenderer.setColor(meshRenderer->getColor());

	collision_channel = Collision_Channel::Enemy;

	xpAmount = 10.0f;
	maxHealth = 10.0f;

	health = maxHealth;

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
}

void CubeEnemy::update(float deltaTime)
{
    glm::vec3 camPos = Camera::mainCamera->transform.position;
    glm::vec3 dirToCamera = glm::normalize(camPos - transform.position);

    glm::vec3 speed = glm::vec3(20);
    glm::vec3 forwardDir = -transform.getForward();
    physicsBody.acceleration = glm::vec3(0.0f);
    physicsBody.acceleration += forwardDir * speed;

    transform.position += physicsBody.velocity * deltaTime;

    glm::quat targetRotation = glm::quatLookAt(dirToCamera, glm::vec3(0, 1, 0));
    float rotationSpeed = 3.0f;
    transform.rotation = glm::slerp(transform.rotation, targetRotation, rotationSpeed * deltaTime);

	// Trail
    /*if (glm::distance(lastPosition, transform.position) > 0.02f)
    {
        TrailMesh* trailMesh = trailRenderer.getMesh();
        glm::vec3 newPos = transform.position;
        trailMesh->addVertex(newPos, 0.1f);
        lastPosition = transform.position;
    }*/

    physicsBody.update();
    trailRenderer.draw(transform.position, transform.rotation, glm::vec3(1.0f));
}
