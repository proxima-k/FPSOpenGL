#include "Enemy.h"
#include <iostream>

Enemy::Enemy(glm::vec3 position, MeshRenderer meshRenderer)
    : Entity(position, meshRenderer)
{
    collision_channel = Collision_Channel::Enemy;

    maxHealth = 10.0f;
    health = maxHealth;

    transform.scale = glm::vec3(.1f);
    transform.position = position;
}