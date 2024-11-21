#pragma once

// controls the flow of boss fight
// spawn boss cage and then spawn boss
// holds reference to both boss cage and the boss
// boss will let this class know when the boss died

// needs reference to player to spawn the cage properly

class BossCage;
class BossEnemy;

class BossFightController {
public:

	void update(float deltaTime);
	void initializeBossFight();
	void notifyBossDied();
	void cleanUp();
	void drawBossCage();
	bool bossFightIsActive() { return hasInitialized; }

private:
	BossCage* cage = nullptr;
	BossEnemy* boss = nullptr;

	bool hasInitialized = false;
};