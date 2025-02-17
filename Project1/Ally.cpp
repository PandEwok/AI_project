#include "Ally.hpp"
#include <cmath>
#include <iostream>

Ally::Ally(float x, float y) : Entity(x, y, sf::Color::Yellow) {}

void Ally::update(float deltaTime, Grid& grid) {
    this->deltaTime = deltaTime;
}

// ChasePlayerAction
ChasePlayerAction::ChasePlayerAction(Ally* ally) : ally(ally) {}

bool ChasePlayerAction::CanExecute() {
    return player.getisAlive();
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
        sf::Vector2f direction = playerPos - allyPos;

        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
}

// GOAPPlanner
GOAPPlanner::GOAPPlanner() {}

std::vector<Action*> GOAPPlanner::Plan(Goal goal, Ally* ally) {
    std::vector<Action*> plan;

    if (goal == Goal::Chase) {
        plan.push_back(new ChasePlayerAction(ally));
    }
    else if (goal == Goal::Defend) {
		std::cout << "Defend\n";
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
    if (player.getIsEnemyNear()) {
        UpdateGoal(Goal::Defend);
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
