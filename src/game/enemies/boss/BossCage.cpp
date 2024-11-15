#include "BossCage.h"

#include <cmath>
#include <glm/glm.hpp>
#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include <behavior_tree/nodes/WaitTask.h>
#include "WallGridRevealTask.h"
#include "FloorGridRevealTask.h"

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

	BT::RootNode* rootNode = new BT::RootNode();
	BT::SequenceNode* sequenceNode = new BT::SequenceNode();
	BT::WaitTask* waitTask = new BT::WaitTask();
	FloorGridRevealTask* floorGridRevealTask = new FloorGridRevealTask();
	WallGridRevealTask* wallGridRevealTask = new WallGridRevealTask();

	behaviorTree.setRootNode(rootNode);
	rootNode->setChild(sequenceNode);
	sequenceNode->addChild(waitTask);
	sequenceNode->addChild(wallGridRevealTask);
	wallGridRevealTask->canRepeat = false;

	// assign key to blackboard for tree nodes to access and manipulate shader uniforms
	behaviorTree.getBlackboard().setValue<WallGrid*>("wallGrid", wallGrid);
	behaviorTree.getBlackboard().setValue<Grid*>("floorGrid", floorGrid);
}

void BossCage::draw()
{
	wallGrid->draw();
	floorGrid->draw();
}

glm::vec3 BossCage::getCellCenterCoords(float angle, float yCoord, glm::vec3& cellNormal) // also add y coordinates
{
	float radians = glm::radians(angle);
	
	float width = xCellCount * cellSize;
	float height = zCellCount * cellSize;

	// to scale the values cosine and sine values for comparison
	float alpha = glm::atan(height / width);
	float cosineScale = glm::cos(glm::radians(45.f)) / glm::cos(alpha);
	float sineScale = glm::sin(glm::radians(45.f)) / glm::sin(alpha);

	float cosine = glm::cos(radians);
	float sine = glm::sin(radians);

	float x, z;
	int xCellCoord, zCellCoord;
	float sign; // for determining if the constant (x or y) is at positive or negative side

	if (glm::abs(cosine * cosineScale) >= glm::abs(sine * sineScale)) {
		sign = std::round(cosine / std::abs(cosine));

		x = width;
		z = width / glm::abs(cosine) * sine;

		xCellCoord = width * sign;
		zCellCoord = z / cellSize;

		if (z < 0) zCellCoord--;
		z = zCellCoord * cellSize + cellSize / 2.f;
		x = xCellCoord;

		cellNormal = glm::vec3(-sign, 0, 0);
	}
	else {
		sign = std::round(sine / std::abs(sine));

		x = height / glm::abs(sine) * cosine;
		z = height;

		zCellCoord = height * sign;
		xCellCoord = x / cellSize;

		if (x < 0) xCellCoord--;
		x = xCellCoord * cellSize + cellSize / 2.f;
		z = zCellCoord;

		cellNormal = glm::vec3(0, 0, -sign);
	}

	float y = (int)(yCoord / cellSize);
	if (yCoord < 0) y--;
	y = y * cellSize + cellSize / 2.f;

	return glm::vec3(x, y, z);
}


glm::vec3 BossCage::getCellCoords(float angle, float yCoord) // also add y coordinates
{
	float radians = glm::radians(angle);

	float width = xCellCount * cellSize;
	float height = zCellCount * cellSize;

	// to scale the values cosine and sine values for comparison
	float alpha = glm::atan(height / width);
	float cosineScale = glm::cos(glm::radians(45.f)) / glm::cos(alpha);
	float sineScale = glm::sin(glm::radians(45.f)) / glm::sin(alpha);

	float cosine = glm::cos(radians);
	float sine = glm::sin(radians);

	float scale = height / glm::abs(sine);
	int shouldXOffset = 1;
	int shouldZOffset = 0;
	if (glm::abs(cosine * cosineScale) >= glm::abs(sine * sineScale)) {
		scale = width / glm::abs(cosine);
		shouldXOffset = 0;
		shouldZOffset = 1;
	}

	float x = scale * cosine;
	float z = scale * sine;

	return glm::vec3(x, yCoord, z);
}
