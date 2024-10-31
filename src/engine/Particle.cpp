#include "Particle.h"

Particle::Particle(glm::vec3 startPos, glm::vec3 scale, Mesh* mesh, Shader* shader, Camera* camera)
    : position(startPos), scale(scale), duration(5.0f), speed(1.0f)
{
    meshRenderer = new MeshRenderer(mesh, shader, camera);
}

void Particle::render()
{
    if(meshRenderer != nullptr)
        meshRenderer->draw(position, rotation, scale);
}
