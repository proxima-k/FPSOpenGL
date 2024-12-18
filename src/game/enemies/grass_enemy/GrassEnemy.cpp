#include "GrassEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <glm/glm.hpp>

#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include "../pillar_enemy/WarningTask.h"
#include "GrassRiseAndFallTask.h"

GrassEnemy::GrassEnemy(glm::vec3 position, float warningTime, float riseTime, float attackTime, float fallTime, float maxHeight, float scaleSpeed)
{
	BT::RootNode* root = new BT::RootNode();
	BT::SequenceNode* sequence = new BT::SequenceNode();
	WarningTask* warningTask = new WarningTask(warningTime, scaleSpeed, true);
	GrassRiseAndFallTask* attackTask = new GrassRiseAndFallTask(riseTime, attackTime, fallTime, maxHeight);

	behaviorTree.setRootNode(root);
	root->setChild(sequence);
	sequence->addChild(warningTask);
	sequence->addChild(attackTask);

	behaviorTree.getBlackboard().setValue<Entity*>("entity", this);

	collision_channel = Collision_Channel::Enemy;

	transform.scale = glm::vec3(0.9f, 0.01f, 0.9f);
	transform.position = position;

	this->meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	this->meshRenderer->setColor(glm::vec3(0.54f, 0.72f, 0.26f));
}
