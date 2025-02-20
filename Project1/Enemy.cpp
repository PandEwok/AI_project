#include "Enemy.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, Color::Red) {}

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

    // Default enemy movement logic can be added here (if needed)
}

void Enemy::checkForAllyCollision(vector<Ally>& allies) {
    for (auto& ally : allies) {
        if (shape.getGlobalBounds().intersects(ally.shape.getGlobalBounds())) {
            if (ally.getisReviving()) {
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
        if (shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
            player.setIsAlive(false);
            return;

        }
}

bool Enemy::isStunned() const {
    return stunTimer > 0;
}
