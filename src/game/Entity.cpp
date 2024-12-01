#include"Entity.h";
#include<iostream>

Entity::Entity(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: transform(position, rotation, scale)
{
	collision_channel = Collision_Channel::None;
	
	// NOTE: for child classes of entity, you have to create your own meshrenderer within the constructor
}

void Entity::update(float deltaTime)
{
}

void Entity::draw() 
{
	if (meshRenderer == nullptr)
	{
		//std::cout << "MeshRenderer is null" << std::endl;
		return;
	}
	meshRenderer->draw(transform);
}