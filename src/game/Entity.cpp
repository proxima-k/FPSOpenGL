#include"Entity.h";
#include<iostream>

Entity::Entity(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: transform(position, rotation, scale)
{
	collision_channel = Collision_Channel::None;

	// note: for child classes of entity, you have to create your own meshrenderer within the constructor
	initMeshRenderer();
}

void Entity::update(float deltaTime)
{
}

void Entity::draw() 
{
	meshRenderer.draw(transform.position, transform.rotation, transform.scale);
}