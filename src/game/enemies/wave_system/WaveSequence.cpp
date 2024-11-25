#include "WaveSequence.h"

#include "GLFW/glfw3.h"

void WaveSequence::update(float dt) {
	for (auto& enemy : enemiesToSpawn) {
		enemy->currentTime -= dt;
	}
}
