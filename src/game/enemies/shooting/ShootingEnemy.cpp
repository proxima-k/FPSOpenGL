#include "ShootingEnemy.h"

#include <iostream>
#include <stdlib.h> 
#include <random>
#include <algorithm>

#include <MeshManager.h>
#include <ShaderManager.h>
#include <array>

#include "../../Game.h"

#include "../EnemyProjectile.h"

ShootingEnemy::ShootingEnemy(glm::vec3 position)
	: Enemy(position), physicsBody(), trailRenderer()
{
	Shader* trailShader = shaderManager->getShader("mesh");
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

    //transform.scale = glm::vec3(1);

	initMeshRenderer();
}


void ShootingEnemy::initMeshRenderer()
{
	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
}

void ShootingEnemy::update(float deltaTime)
{
    glm::vec3 camPos = Camera::mainCamera->transform.position;
    glm::vec3 dirToCamera = glm::normalize(camPos - transform.position);

    glm::quat targetRotation = glm::quatLookAt(dirToCamera, glm::vec3(0, 1, 0));
    float rotationSpeed = 3.0f;
    transform.rotation = glm::slerp(transform.rotation, targetRotation, rotationSpeed * deltaTime);

    switch (state)
    {
    case EnemyState::Chase:
        chase_player(deltaTime);
        break;
    case EnemyState::Dodge:
        dodge_player(deltaTime);
        break;
    }
    // trail system

    if (glm::distance(lastPosition, transform.position) > 0.02f)
    {
        TrailMesh* trailMesh = trailRenderer.getMesh();
        glm::vec3 newPos = transform.position;
        trailMesh->addVertex(newPos, 0.1f);
        lastPosition = transform.position;
    }

    physicsBody.update();
    trailRenderer.draw(transform.position, transform.rotation, glm::vec3(1.0f));
}

void ShootingEnemy::chase_player(float dt)
{
    glm::vec3 camPos = Camera::mainCamera->transform.position;

    glm::vec3 speed = glm::vec3(40);
    glm::vec3 forwardDir = -transform.getForward();
    physicsBody.acceleration = glm::vec3(0.0f);
    physicsBody.acceleration += forwardDir * speed;

    transform.position += physicsBody.velocity * dt;

    if (glm::distance(transform.position, camPos) < 4) {
        filledList = false;
        state = EnemyState::Dodge;
    }
}

void ShootingEnemy::dodge_player(float dt)
{
    if (target_positions.empty() && filledList) {
        state = EnemyState::Chase;
        target_positions.clear();
        meshRenderer->setColor( { 1, 1, 1 } );
        return;
    }

    if (target_positions.empty() && !filledList) { 
        std::vector<glm::vec3> directions = get_random_directions();
        for (glm::vec3 dir : directions) {
            target_positions.push_back(transform.position + (dir * glm::vec3(3)));
        }

        filledList = true;
    }

    if (!target_positions.empty()) {
        transform.position = glm::mix(transform.position, target_positions.back(), 0.55f * dt);
        float dist = glm::distance(transform.position, target_positions.back());

        const float switchColorAt = 2.15f;
        const float arrivalDist = 2;

        glm::vec3 color = dist < switchColorAt ? glm::vec3(1, 0, 0) : glm::vec3(1, 1, 1);
        meshRenderer->setColor(color);

        if (dist < arrivalDist) {
            target_positions.pop_back();
            auto projectile = game->spawn_entity<EnemyProjectile>(transform.position);
            projectile->launch(transform.position, transform.getForward(), transform.getUp());
        }
    }
}

std::vector<glm::vec3> ShootingEnemy::get_random_directions()
{
    int randomDir = rand() % 4 + 1;

    std::array<glm::vec3, 4> directions = {
        transform.getUp(), 
       -transform.getUp(), 
        transform.getRight(), 
       -transform.getRight()
    };

    std::vector<glm::vec3> directionHolder;

    const int directionsToReturnCount = 4;

    std::sample(directions.begin(), directions.end(), std::back_inserter(directionHolder), directionsToReturnCount,
        std::mt19937{ std::random_device{}() });

    return directionHolder;
}
