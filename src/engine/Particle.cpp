#include "Particle.h"

#include <random>
#include "xyzmath.h"

Particle::Particle(glm::vec3 startPos, glm::vec3 scale, const float dur, const float speed, Mesh* mesh, Shader* shader, Camera* camera)
    : position(startPos), scale(scale), initialScale(scale), duration(dur), currentDuration(duration), speed(speed)
{
    meshRenderer = new MeshRenderer(mesh, shader, camera);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f);
    std::uniform_real_distribution<float> durationDist(0.5f, 1.5f);

    glm::vec3 randomEuler(dist(gen), dist(gen), dist(gen));

    // apply a slight offset for scale in the particles!
    scale = initialScale *= scaleDist(gen);

    // apply a slight offset for duration in the particles!
    currentDuration = duration *= durationDist(gen);

    rotation = glm::quat(randomEuler);
}

void Particle::render()
{
    if(meshRenderer != nullptr)
        meshRenderer->draw(position, rotation, scale);
}

void Particle::update(float dt)
{
    bDestroyed = currentDuration <= 0 || (currentDuration -= dt, false);

    float progress = 1.0f - (currentDuration / duration);
    progress = progress - std::floor(progress);

    glm::vec3 easedProgress = xyzmath::EaseOutCubicBounceVec3(glm::vec3(progress));

    scale = initialScale * (easedProgress + glm::vec3(1.0f));

    auto forward = glm::normalize(rotation * glm::vec3(0, 0, 1));
    position += forward * speed * dt;
}

