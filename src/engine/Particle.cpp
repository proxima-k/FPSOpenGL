#include "Particle.h"

#include <random>

Particle::Particle(glm::vec3 startPos, glm::vec3 scale, const float duration, Mesh* mesh, Shader* shader, Camera* camera)
    : position(startPos), scale(scale), duration(duration), speed(1.0f)
{
    meshRenderer = new MeshRenderer(mesh, shader, camera);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, glm::two_pi<float>());

    glm::vec3 randomEuler(dist(gen), dist(gen), dist(gen));
    rotation = glm::quat(randomEuler);
}

void Particle::render()
{
    if(meshRenderer != nullptr)
        meshRenderer->draw(position, rotation, scale);
}

void Particle::update(float dt)
{
    auto forward = glm::normalize(rotation * glm::vec3(0, 0, 1));
    position += forward * 5.0f * dt;

    duration -= dt;

    if (duration <= 0)
        bDestroyed = true;
}
