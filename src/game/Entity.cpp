#include"Entity.h";
#include<iostream>

Entity::Entity(Mesh* mesh, Shader* shader, Camera* camera)
	: Entity()
{
	meshToDraw = mesh;
	meshToDraw->setShader(shader);
	meshToDraw->setCamera(camera);
	std::cout << "Pos : " << transform.position.x << transform.position.y << transform.position.z << "\nScale : " << transform.scale.x << transform.scale.y << transform.scale.z << "\nMesh : " << meshToDraw << "\nCamera :" << camera << "\nShader :" << shader << std::endl;
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