#include "Ally.hpp"
#include <cmath>
#include <iostream>

Ally::Ally(float x, float y) : Entity(x, y, sf::Color::Yellow) {}

void Ally::update(float deltaTime, Grid& grid) {
    this->deltaTime = deltaTime;
}

bool Ally::getClosePlayer() {
	return closePlayer;
}

void Ally::setClosePlayer(bool closePlayer) {
	this->closePlayer = closePlayer;
}

void Ally::startReviving() {
    isReviving = true;
}

void Ally::stopReviving() {
    isReviving = false;
    shape.setOutlineThickness(0);
}

bool Ally::isRevivalComplete() {
    return reviveProgress >= REVIVE_TIME;
}

void Ally::resetReviveProgress() {
    reviveProgress = 0.0f;
}

void Ally::setReviveProgress(float reviveProgress) {
	this->reviveProgress = reviveProgress;
}

float Ally::getReviveProgress() {
	return reviveProgress;
}



// ChasePlayerAction
ChasePlayerAction::ChasePlayerAction(Ally* ally) : ally(ally) {}

bool ChasePlayerAction::CanExecute() {
    return 1;
}

void ChasePlayerAction::Execute() {
    if (!ally) {
        std::cout << "Error: ChasePlayerAction has a null ally reference!\n";
        return;
    }

    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    float distance = std::sqrt(std::pow(playerPos.x - allyPos.x, 2) + std::pow(playerPos.y - allyPos.y, 2));
    if (distance > 75) {
        ally->setClosePlayer(false);
        sf::Vector2f direction = playerPos - allyPos;

        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
    else
        ally->setClosePlayer(true);

}

RevivePlayerAction::RevivePlayerAction(Ally* ally) : ally(ally) {}

bool RevivePlayerAction::CanExecute() {
    return !player.getisAlive(); // Only execute if the player is dead
}

void RevivePlayerAction::Execute() {
    if (!ally) {
        std::cout << "Error: RevivePlayerAction has a null ally reference!\n";
        return;
    }

    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    // Step 1: Move toward the player first (reuse ChasePlayerAction logic)
    ChasePlayerAction chase(ally);
    if (!ally->getClosePlayer()) {
        chase.Execute();  // Move toward the player
        return;
    }

    // Step 2: Start revival process
    ally->startReviving();

    // Step 3: Progress revive timer if not interrupted
    //if (!player.getIsEnemyNear()) {
        ally->setReviveProgress(ally->getReviveProgress() + deltaTime);

        // Flash color to indicate revival
        float progress = ally->getReviveProgress();
        if (static_cast<int>(progress * 5) % 2 == 0) {
            ally->shape.setOutlineThickness(3);
			ally->shape.setOutlineColor(sf::Color::Black);
        }
        else {
            ally->shape.setFillColor(sf::Color::Yellow);
            ally->shape.setOutlineThickness(3);
            ally->shape.setOutlineColor(sf::Color::White);
        }

    //}
    //else {
    //    ally->stopReviving(); // Pauses revive but does not reset progress
    //}

    // Step 4: If 3 seconds passed, revive the player
    if (ally->isRevivalComplete()) {
        //player.revive();  // Implement this function in `Player`
        std::cout << "Player revived!\n";
        player.setIsAlive(true);
        ally->resetReviveProgress();
        ally->shape.setOutlineThickness(0);
        ally->shape.setFillColor(sf::Color::Yellow);  // Reset color
    }
}





// DefendPlayerAction
DefendPlayerAction::DefendPlayerAction(Ally* ally) : ally(ally) {}

bool DefendPlayerAction::CanExecute() {
    return player.getIsEnemyNear();
}

void DefendPlayerAction::Execute() {
    if (!ally) {
        std::cout << "Error: DefendPlayerAction has a null ally reference!\n";
        return;
    }

    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f enemyPos = player.getNearestEnemyPosition();
    sf::Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    // Ensure we have a valid enemy position
    if (enemyPos == sf::Vector2f(0.f, 0.f)) {
        std::cout << "No enemy found, cannot defend!\n";
        return;
    }

    // Calculate the correct defend position BETWEEN player and enemy
    sf::Vector2f defendPosition = playerPos + (enemyPos - playerPos) * 0.35f;  // Midpoint

    // Move ally towards defendPosition
    sf::Vector2f direction = defendPosition - allyPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;  // Normalize direction
    }

    ally->shape.move(direction * Ally::SPEED * deltaTime);
}



// GOAPPlanner
GOAPPlanner::GOAPPlanner() {}

std::vector<Action*> GOAPPlanner::Plan(Goal goal, Ally* ally) {
    std::vector<Action*> plan;

    if (goal == Goal::Chase) {
        plan.push_back(new ChasePlayerAction(ally));
    }
    else if (goal == Goal::Defend) {
        plan.push_back(new DefendPlayerAction(ally));
    }
    else if (goal == Goal::Revive) {
		plan.push_back(new RevivePlayerAction(ally));
    }
    else {
        std::cout << "Error: Unknown goal\n";
    }
    return plan;
}

// GOAPAgent
GOAPAgent::GOAPAgent(Ally* ally) : owner(ally), goal(Goal::Chase) {
    UpdatePlan();
}

void GOAPAgent::UpdateGoal(Goal newGoal) {
    if (goal != newGoal) {
        goal = newGoal;
        UpdatePlan();
    }
}

void GOAPAgent::UpdatePlan() {
    plan = planner.Plan(goal, owner);
}

void GOAPAgent::PerformActions() {
    if (player.getIsEnemyNear() && player.getisAlive()) {
        UpdateGoal(Goal::Defend);
    }
    else if (player.getisAlive()) {
        UpdateGoal(Goal::Chase);
    }
	else if (!player.getisAlive()) {
		UpdateGoal(Goal::Revive);
	}

    for (auto& action : plan) {
        if (action->CanExecute()) {
            action->Execute();
        }
        else {
            std::cout << "Action impossible: " << typeid(*action).name() << "\n";
        }
    }
}

std::vector<Action*> GOAPAgent::getPlan() {
    return plan;
}
