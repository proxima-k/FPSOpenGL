#include "Enemy.h"
#include <iostream>

Enemy::Enemy(glm::vec3 position)
    : Entity(position)
{
    collision_channel = Collision_Channel::Enemy;

    transform.scale = glm::vec3(.2f);
    transform.position = position;

    // make this more visually clear
    // maybe some elastic bouncing curves would be good
    // or maybe some type of crazy particle explosion?
    // audio play?
    game->spawn_particle_source(transform.position, 60.0f, 0.6f, 2.0f);
}