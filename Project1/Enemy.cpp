#include "Enemy.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, Color::Red) {}

sf::Vector2f Enemy::getPosition(){
    return shape.getPosition();
}

void Enemy::update(float deltaTime, Grid& grid) {
    if (stunTimer > 0) {
        stunTimer -= deltaTime;

        if (static_cast<int>(stunTimer * 5) % 2 == 0) {
            shape.setOutlineColor(Color::Green);
            shape.setOutlineThickness(3);
        }
        else {
            shape.setOutlineThickness(0);
        }

        if (stunTimer <= 0) {
            shape.setOutlineThickness(0);
            shape.setFillColor(Color::Red);
        }

        return;
    }

    if (!player.getisAlive()) {
        // Player is dead, seek revenge on allies
        Ally* closestAlly = nullptr;
        float minDistance = DETECTION_RADIUS; // Set a detection range

        for (auto& ally : allies) {
            if (!ally.isAllyAlive()) continue; // Skip dead allies

            float distance = std::sqrt(std::pow(ally.shape.getPosition().x - shape.getPosition().x, 2) +
                std::pow(ally.shape.getPosition().y - shape.getPosition().y, 2));

            if (distance < minDistance) {
                minDistance = distance;
                closestAlly = &ally;
            }
        }

        if (closestAlly) {
            // Move toward the closest ally
            sf::Vector2f direction = closestAlly->shape.getPosition() - shape.getPosition();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) direction /= length; // Normalize

            shape.move(direction * SPEED * deltaTime);
        }
        // Default enemy movement logic can be added here (if needed)
    }
}

void Enemy::checkForAllyCollision(vector<Ally>& allies) {
    for (auto& ally : allies) {
        if (shape.getGlobalBounds().intersects(ally.shape.getGlobalBounds())) {
            if (ally.getisVulnerable()) {
                cout << "Ally was killed while reviving!\n";
                ally.setAllyAlive(false);  // Kill the ally
            }
            else {
                cout << "Enemy stunned by Ally!\n";
                stunTimer = STUN_DURATION;
            }
            return;

        }
    }
}

void Enemy::checkForPlayerCollision(Player& player) {
    if (!player.isPlayerInvincible() && shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
        player.setIsAlive(false);
    }
}


bool Enemy::isStunned() const {
    return stunTimer > 0;
}

