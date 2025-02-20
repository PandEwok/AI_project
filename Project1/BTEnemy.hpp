#pragma once

#include "BTNode.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class BTEnemy : public Enemy {
protected:
	float attackRange = 40;
	float dectectionRange = 40 * 6;
	Blackboard blackboard;
	unique_ptr<SelectorNode> root = make_unique<SelectorNode>();
public:
	BTEnemy(float x, float y);
	void update(float deltaTime, Grid& grid) override;
};