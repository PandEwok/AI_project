#include "BigEnemy.hpp"
#include <iostream>

BigEnemy::BigEnemy(float x, float y) : Enemy(x, y) {}

void BigEnemy::update(float deltaTime, Grid& grid, std::vector<Ally>& allies) {
	shape.setSize(sf::Vector2f(80, 80));
    Enemy::update(deltaTime, grid);  // Call base update (handles stun logic)
    checkForAllyCollision(allies);   // Ensure big enemies also get stunned

    if (isStunned()) return;  // Skip movement if stunned

    //UPDATE
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f myPos = shape.getPosition();

    // Calculate distance to player
    float distance = std::sqrt(std::pow(playerPos.x - myPos.x, 2) + std::pow(playerPos.y - myPos.y, 2));

    if (distance < DETECTION_RADIUS) {  // If player is within range, move toward them
        sf::Vector2f direction = playerPos - myPos;

        // Normalize the direction vector
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }

        // Move towards the player
        if (player.getisAlive()) {
            shape.move(direction * SPEED * deltaTime);
        }
    }

    // Normal enemy update logic (move, attack, etc.)

    checkForAllyCollision(allies);
    checkForPlayerCollision(player);
}