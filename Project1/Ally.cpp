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

bool Ally::getisReviving() {
	return isReviving;
}

bool Ally::isAllyAlive() {
	return isAlive;
}

void Ally::setAllyAlive(bool alive) {
	isAlive = alive;
}

// ChasePlayerAction
ChasePlayerAction::ChasePlayerAction(Ally* ally) : ally(ally) {}

bool ChasePlayerAction::CanExecute() {
    return 1;
}

void ChasePlayerAction::Execute() {
	std::cout << player.getisAlive() << std::endl;
    if (!ally) {
        std::cout << "Error: ChasePlayerAction has a null ally reference!\n";
        return;
    }

    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    const float MIN_DISTANCE = 20.0f;  // Minimum distance from Player
    const float MAX_DISTANCE = 50.0f; // Max distance before moving in

    float distance = std::sqrt(std::pow(playerPos.x - allyPos.x, 2) + std::pow(playerPos.y - allyPos.y, 2));

    if (distance > MAX_DISTANCE) {
        // Too far? Move closer.
        ally->setClosePlayer(false);
        sf::Vector2f direction = playerPos - allyPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
    else if (distance < MIN_DISTANCE) {
		ally->setClosePlayer(true);
        // Too close? Push away from the Player.
        sf::Vector2f direction = allyPos - playerPos; // Reverse direction
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
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

    float distance = std::sqrt(std::pow(playerPos.x - allyPos.x, 2) + std::pow(playerPos.y - allyPos.y, 2));
    const float MIN_DISTANCE = 45.0f;

    if (distance > MIN_DISTANCE) {
        std::cout << "Moving closer to revive Player...\n";
        sf::Vector2f direction = playerPos - allyPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
        return; // Don't start reviving yet!
    }

    // If close enough, start reviving
    ally->setClosePlayer(true);
    ally->startReviving();
    ally->setReviveProgress(ally->getReviveProgress() + deltaTime);

    // Flash color effect
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

    // If revival is complete, bring the player back
    if (ally->isRevivalComplete()) {
        std::cout << "Player revived!\n";
        player.setIsAlive(true);
        ally->resetReviveProgress();
        ally->shape.setOutlineThickness(0);
        ally->shape.setFillColor(sf::Color::Yellow);
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

    if (enemyPos == sf::Vector2f(0.f, 0.f)) {
        std::cout << "No enemy found, cannot defend!\n";
        return;
    }

    sf::Vector2f defendPosition = playerPos + (enemyPos - playerPos) * 0.35f;

    const float MIN_DISTANCE = 20.0f;
    const float MAX_DISTANCE = 20.0f;

    sf::Vector2f direction = defendPosition - allyPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > MAX_DISTANCE) {
        // Move closer
        direction /= length;
        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
    else if (length < MIN_DISTANCE) {
        // Move away slightly
        direction = allyPos - playerPos;
        length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
}

DeathAction::DeathAction(Ally* ally) : ally(ally) {}
bool DeathAction::CanExecute(){
    return !ally->isAllyAlive(); // Execute only if the ally is dead
}

void DeathAction::Execute() {
    std::cout << "Ally has died. No further actions can be taken.\n";
    ally->shape.setFillColor(sf::Color(130, 175, 130));; // Change color to indicate death
    ally->shape.setOutlineThickness(0);
}





// GOAPPlanner
GOAPPlanner::GOAPPlanner() {}

std::vector<Action*> GOAPPlanner::Plan(Goal goal, Ally* ally) {
    std::vector<Action*> plan;

    if (goal == Goal::Revive) {
        plan.push_back(new RevivePlayerAction(ally));
    }
    else if (goal == Goal::Chase) {
        plan.push_back(new ChasePlayerAction(ally));
    }
    else if (goal == Goal::Defend) {
        plan.push_back(new DefendPlayerAction(ally));
    }
	else if (goal == Goal::Death) {
		plan.push_back(new DeathAction(ally));
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
    
	if (!owner->isAllyAlive()) {
		UpdateGoal(Goal::Death);
	}
    else if (player.getIsEnemyNear() && player.getisAlive()) {
        UpdateGoal(Goal::Defend);
    }
    else if (!player.getisAlive()) {
        UpdateGoal(Goal::Revive);
    }
    else if (player.getisAlive()) {
        UpdateGoal(Goal::Chase);
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
