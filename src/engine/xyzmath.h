#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace xyzmath
{
    inline float EaseOutElastic(float t) {
        const float c4 = (2 * M_PI) / 2.5f;

        if (t == 0) return 0;
        if (t == 1) return 1;

        return pow(2, -12 * t) * sin((t * 12 - 0.75f) * c4) + 1;
    }

    inline glm::vec3 EaseOutCubicBounceVec3(const glm::vec3& v) {
        auto EaseOutCubicBounce = [](float x) {
            return x < 0.5f ?
                2 * std::pow(x * 2, 3) / 2 :
                2 * (1 - std::pow(2 * (x - 0.5f), 3)) / 2;
            };

        return glm::vec3(
            EaseOutCubicBounce(v.x),
            EaseOutCubicBounce(v.y),
            EaseOutCubicBounce(v.z)
        );
    }
}