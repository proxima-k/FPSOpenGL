#include "Particle.h"

Particle::Particle(glm::vec3 startPos, Mesh* mesh, Shader* shader, Camera* camera)
    : position(startPos), duration(5.0f), speed(1.0f)
{
    meshRenderer = MeshRenderer(mesh, shader, camera);
}

void Particle::render()
{
    meshRenderer.draw(position, rotation, scale);
}
