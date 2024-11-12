#include "BossCage.h"

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
