#include "BossCage.h"

#include <cmath>
#include <glm/glm.hpp>

BossCage::BossCage(glm::vec3 centerWorldPosition, int xCellCount, int yCellCount, int zCellCount, float cellSize)
	: centerWorldPosition(centerWorldPosition), xCellCount(xCellCount), yCellCount(yCellCount), zCellCount(zCellCount), cellSize(cellSize)
{
	minBounds.x = centerWorldPosition.x - xCellCount * cellSize;
	minBounds.y = centerWorldPosition.z - zCellCount * cellSize;
	maxBounds.x = centerWorldPosition.x + xCellCount * cellSize;
	maxBounds.y = centerWorldPosition.z + zCellCount * cellSize;
	// floor grid will take x and z cell Count
	// wall grid will take xyz cell Count

	centerWorldPosition.y = 0;
	wallGrid = new WallGrid(cellSize, xCellCount, zCellCount, yCellCount);
	floorGrid = new Grid(xCellCount, zCellCount, cellSize);

	wallGrid->transform.position = centerWorldPosition;
	floorGrid->transform.position = centerWorldPosition;

	// assign key to blackboard for tree nodes to access and manipulate shader uniforms
	behaviorTree.getBlackboard().setValue<WallGrid*>("wallGrid", wallGrid);
	behaviorTree.getBlackboard().setValue<Grid*>("wallGrid", floorGrid);

}

void BossCage::draw()
{
	wallGrid->draw();
	floorGrid->draw();
}

void BossCage::getCellCoords(float angle)
{
	float radians = glm::radians(angle);
	float width = xCellCount * cellSize * 2;
	float height = zCellCount * cellSize * 2;

	width = 10;
	height = 10;

	float scale = width / glm::max(glm::abs(glm::cos(radians)), glm::abs(glm::sin(radians)));
	float x = glm::cos(radians) * scale;
	float y = glm::sin(radians) * scale;

	//std::cout << "x: " << width << " y: " << height << std::endl;
	std::cout << "x: " << x << " y: " << y << std::endl;
}
