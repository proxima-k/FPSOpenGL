#include "Enemy.h"
#include <iostream>

Enemy::Enemy(glm::vec3 position)
    : Entity(position)
{
    collision_channel = Collision_Channel::Enemy;

    transform.scale = glm::vec3(.2f);
    transform.position = position;
}