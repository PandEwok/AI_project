#include "Ally.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

Ally::Ally(float x, float y) : Entity(x, y, sf::Color::Yellow) {}

void Ally::update(float deltaTime, Grid& grid) {
    this->deltaTime = deltaTime;  // Store the latest deltaTime
}



//ProtectPlayerAction
bool ProtectPlayerAction::CanExecute() {
    return player.getisAlive();  // Peut chercher de la nourriture si l'agent n'en a pas
}
void ProtectPlayerAction::Execute() {
    if (!ally) {
        std::cout << "Error: ProtectPlayerAction has a null ally reference!\n";
        return;
    }

    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;  // Get the stored deltaTime

    float distance = std::sqrt(std::pow(playerPos.x - allyPos.x, 2) + std::pow(playerPos.y - allyPos.y, 2));
    if (distance > 75) {
        sf::Vector2f direction = playerPos - allyPos;

        // Normalize direction
        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        // Apply deltaTime for smooth movement
        ally->shape.move(direction * Ally::SPEED * deltaTime);

        //std::cout << "Ally moving towards player. Distance: " << distance << "\n";
    }
}





GOAPPlanner::GOAPPlanner() {};

std::vector<Action*> GOAPPlanner::Plan(Goal& goal, Ally* ally) {
    std::vector<Action*> plan;
    if (goal == Goal::Defend) {
        plan.push_back(new ProtectPlayerAction(ally));
    }
    return plan;
}


void GOAPAgent::PerformActions() {
    for (auto& action : plan) {
        if (action->CanExecute()) {
            action->Execute();  // Pass deltaTime
        }
        else {
            std::cout << "Action impossible: " << typeid(*action).name() << "\n";
        }
    }
}


std::vector<Action*> GOAPAgent::getPlan() {
	return plan;
}