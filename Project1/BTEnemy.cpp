#include "BTEnemy.hpp"

BTEnemy::BTEnemy(float x, float y) : Enemy(x,y)
{
	blackboard.SetValue("InRange", 0);
	blackboard.SetValue("PlayerDetected", 0);
	blackboard.SetValue("NearAlly", 0);

	unique_ptr<SequenceNode> attackSequence = make_unique<SequenceNode>();
	attackSequence->AddChild(make_unique<ConditionNode>(blackboard, "InRange", 1));
	attackSequence->AddChild(make_unique<ActionNode>("attack"));

	unique_ptr<SequenceNode> followSequence = make_unique<SequenceNode>();
	followSequence->AddChild(make_unique<ConditionNode>(blackboard, "PlayerDetected", 1));
	followSequence->AddChild(make_unique<FollowActionNode>(*this, player));

	unique_ptr<SequenceNode> fleeSequence = make_unique<SequenceNode>();
	fleeSequence->AddChild(make_unique<ConditionNode>(blackboard, "NearAlly", 1));
	fleeSequence->AddChild(make_unique<ActionNode>("flee"));

	root->AddChild(move(fleeSequence));
	root->AddChild(move(attackSequence));
	root->AddChild(move(followSequence));
}

void BTEnemy::update(float deltaTime, Grid& grid)
{
	checkForAllyCollision(allies);   // Ensure big enemies also get stunned*

	if (isStunned()) {
		shape.setFillColor(Color(70, 70, 70));
		shape.setOutlineColor(Color::Red);
		shape.setOutlineThickness(2);
		waypoints.clear();
		return;
	}
	checkForPlayerCollision(player);

	root->execute();
	if (abs(player.shape.getPosition().x - shape.getPosition().x) <= attackRange and abs(player.shape.getPosition().y - shape.getPosition().y) <= attackRange) {
		blackboard.SetValue("InRange", 1);
	}
	else { blackboard.SetValue("InRange", 0); }

	if (abs(player.shape.getPosition().x - shape.getPosition().x) <= dectectionRange and abs(player.shape.getPosition().y - shape.getPosition().y) <= dectectionRange) {
		blackboard.SetValue("PlayerDetected", 1);
	}
	else {
		blackboard.SetValue("PlayerDetected", 0);
		waypoints.clear();
	}

	for (auto& ally : allies) {
		if (ally.shape.getPosition().x - shape.getPosition().x <= 40*2 and ally.shape.getPosition().y - shape.getPosition().y <= 40 * 2) {
			blackboard.SetValue("NearAlly", 1);
			break;
		}
		else blackboard.SetValue("NearAlly", 0);
	}
}
