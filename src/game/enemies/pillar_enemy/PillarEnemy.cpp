#include "PillarEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <iostream>
#include <glm/glm.hpp>

#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include "WarningTask.h"
#include "AttackTask.h"


PillarEnemy::PillarEnemy(glm::vec3 position)
	: Entity(position)
{
	// setup nodes for behavior tree
	BT::RootNode* root = new BT::RootNode();
	BT::SequenceNode* sequence = new BT::SequenceNode();
	WarningTask* warningTask = new WarningTask();
	AttackTask* attackTask = new AttackTask();

	behaviorTree.setRootNode(root);
	root->setChild(sequence);
	sequence->addChild(warningTask);
	sequence->addChild(attackTask);

	behaviorTree.getBlackboard().setValue<Entity*>("entity", this);

    collision_channel = Collision_Channel::Enemy;

	transform.scale = glm::vec3(0.9f, 0.01f, 0.9f);
    transform.position = position;

	initMeshRenderer();
}

void PillarEnemy::initMeshRenderer()
{
	this->meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	this->meshRenderer->setColor(glm::vec3(1.f, 0.5f, 0.5f));
}