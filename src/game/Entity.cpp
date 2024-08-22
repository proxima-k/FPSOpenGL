#include"Entity.h";
#include<iostream>

Entity::Entity(glm::vec3 position, MeshRenderer meshRenderer)
	: Entity()
{
	this->meshRenderer = meshRenderer;
	transform.position = position;
	collision_channel = Collision_Channel::None;

	//std::cout << "Pos : " << transform.position.x << transform.position.y << transform.position.z << "\nScale : " << transform.scale.x << transform.scale.y << transform.scale.z << "\nMesh : " << meshToDraw << "\nCamera :" << camera << "\nShader :" << shader << std::endl;
}

void Entity::update(float deltaTime)
{
}

void Entity::draw() 
{
	meshRenderer.draw(transform.position, transform.rotation, transform.scale);
}