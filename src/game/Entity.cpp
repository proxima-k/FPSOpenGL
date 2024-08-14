#include"Entity.h";

Entity::Entity(Mesh* mesh, Shader* shader, Camera* camera)
	: Entity()
{
	meshToDraw = mesh;
	meshToDraw->setShader(shader);
	meshToDraw->setCamera(camera);
}

void Entity::update()
{

}

void Entity::draw() 
{
	if (meshToDraw != nullptr)
		meshToDraw->draw(transform.position, transform.rotation, transform.scale);
}