#include "Enemy.h"
#include <iostream>

Enemy::Enemy(glm::vec3 position, MeshRenderer meshRenderer)
    : Entity(position, meshRenderer)
{
    collision_channel = Collision_Channel::Enemy;

    transform.scale = glm::vec3(.1f);
    transform.position = position;
}