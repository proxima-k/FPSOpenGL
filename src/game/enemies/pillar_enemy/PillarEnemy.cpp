#include "PillarEnemy.h"

#include <iostream>
#include <glm/glm.hpp>
#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include "WarningTask.h"
#include "AttackTask.h"

PillarEnemy::PillarEnemy(glm::vec3 position, MeshRenderer meshRenderer)
    : Entity(position, meshRenderer)
{
	// create nodes for the behavior tree

	behaviorTree.getBlackboard().setValue<Entity*>("entity", this);

	WarningTask* warningTask = new WarningTask();
	AttackTask* attackTask = new AttackTask();

	BT::SequenceNode* sequence = new BT::SequenceNode();
	sequence->addChild(warningTask);
	sequence->addChild(attackTask);

	BT::RootNode* root = new BT::RootNode();
	root->setChild(sequence);

	behaviorTree.setRootNode(root);

    collision_channel = Collision_Channel::Enemy;

	transform.scale = glm::vec3(0.5f, 0.01f, 0.5f);
    transform.position = position;
    //currentHeight = 0.01f;
	//updateHeight();

	this->meshRenderer.setColor(glm::vec3(1.f, 0.3f, 0.3f));
}
/*
void PillarEnemy::update(float deltaTime)
{
	waitTimer -= deltaTime;
	if (waitTimer > 0.f)
	{
		return;
	}

	//currentHeight = glm::mix(currentHeight, 2.f, 0.03f);
	if (currentHeight >= 2.f)
	{
		//currentHeight = 0.01f;
		return;
	}
	currentHeight += 10.f * deltaTime;
	updateHeight();

	/*std::cout << "======================" << std::endl;
	std::cout << "current height: " << currentHeight << std::endl;
	std::cout << "transform scale: " << transform.scale.y << std::endl;
	std::cout << "y position: " << transform.position.y << std::endl;
}


void PillarEnemy::updateHeight()
{
	transform.scale.y = currentHeight;
	transform.position.y = currentHeight;
}
*/
