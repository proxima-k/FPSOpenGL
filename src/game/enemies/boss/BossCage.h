#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../../Grid.h"
#include "../../WallGrid.h"

#include <behavior_tree/BehaviorTree.h>

class BossCage {
public:
	BossCage(glm::vec3 centerWorldPosition, int xCellCount, int yCellCount, int zCellCount, float cellSize);
	~BossCage() { delete wallGrid; delete floorGrid; }

	void update(float deltaTime) { behaviorTree.update(deltaTime); }
	void draw(); // handles drawing grid?
	void getCellCoords(float angle);

	float cellSize;
	float xCellCount, yCellCount, zCellCount;   // these are all half of the total cells per axis
	glm::vec2 minBounds;						// minimum point of the cage
	glm::vec2 maxBounds;						// maximum point of the cage
	glm::vec3 centerWorldPosition;

	WallGrid* wallGrid = nullptr;
	Grid* floorGrid = nullptr;

private:

	BT::BehaviorTree behaviorTree;		// for animating floor and wall grid
};