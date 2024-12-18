#include "WaveEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <glm/glm.hpp>

#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include "../pillar_enemy/WarningTask.h"
#include "WaveRiseAndFallTask.h"

WaveEnemy::WaveEnemy(glm::vec3 position)
{
	BT::RootNode* root = new BT::RootNode();
	BT::SequenceNode* sequence = new BT::SequenceNode();
	WarningTask* warningTask = new WarningTask();
	WaveRiseAndFallTask* attackTask = new WaveRiseAndFallTask(1.2f, 5.f);

	behaviorTree.setRootNode(root);
	root->setChild(sequence);
	sequence->addChild(warningTask);
	sequence->addChild(attackTask);

	behaviorTree.getBlackboard().setValue<Entity*>("entity", this);

	collision_channel = Collision_Channel::Enemy;

	transform.scale = glm::vec3(0.9f, 0.01f, 0.9f);
	transform.position = position;

	this->meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	this->meshRenderer->setColor(glm::vec3(0.5f, 0.5f, 1.f));
}
