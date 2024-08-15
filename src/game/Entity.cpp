#include"Entity.h";
#include<iostream>

Entity::Entity(glm::vec3 position, Mesh* mesh, Shader* shader, Camera* camera)
	: Entity()
{
	meshToDraw = mesh;
	meshToDraw->setShader(shader);
	meshToDraw->setCamera(camera);
	transform.position = position;
	collision_channel = Collision_Channel::Enemy;

	//std::cout << "Pos : " << transform.position.x << transform.position.y << transform.position.z << "\nScale : " << transform.scale.x << transform.scale.y << transform.scale.z << "\nMesh : " << meshToDraw << "\nCamera :" << camera << "\nShader :" << shader << std::endl;
}

void Entity::update()
{
}

void Entity::draw() 
{
	if (meshToDraw != nullptr) 
	{
		meshToDraw->draw(transform.position, transform.rotation, transform.scale);
	}
}