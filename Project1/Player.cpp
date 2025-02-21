#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(float x, float y) : Entity(x, y, Color::Blue) { SPEED = 250.0f; }

void Player::update(float deltaTime, Grid& grid) {
    if (isAlive) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            isAlive =false;
        }
        // Handle Invincibility
        if (isInvincible) {
            invincibilityTimer -= deltaTime;
            if (invincibilityTimer <= 0.0f) {
                isInvincible = false;
                shape.setFillColor(sf::Color::Blue); // Reset color after invincibility
                shape.setOutlineThickness(0);
            }
            else {
                // Blinking effect to indicate invincibility
                if (static_cast<int>(invincibilityTimer * 10) % 2 == 0) {
                    shape.setFillColor(sf::Color::Transparent);
                    shape.setOutlineColor(sf::Color::Blue);
                    shape.setOutlineThickness(1);

                }
                else {
                    shape.setFillColor(sf::Color::Blue);
                    shape.setOutlineThickness(0);

                }
            }
        }
        else {
            shape.setFillColor(sf::Color::Blue);
        }

        // Movement logic
        Vector2f movement(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Z)) movement.y -= SPEED * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::S)) movement.y += SPEED * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::Q)) movement.x -= SPEED * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::D)) movement.x += SPEED * deltaTime;

        Vector2f newPosition = shape.getPosition() - shape.getSize() / 2.f + movement;
        FloatRect newBounds(newPosition, shape.getSize());

        auto isWalkable = [&](float x, float y) {
            int gridX = static_cast<int>(x / CELL_SIZE);
            int gridY = static_cast<int>(y / CELL_SIZE);
            return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).walkable;
            };

        if (isWalkable(newBounds.left, newBounds.top) &&
            isWalkable(newBounds.left + newBounds.width - 1, newBounds.top) &&
            isWalkable(newBounds.left, newBounds.top + newBounds.height - 1) &&
            isWalkable(newBounds.left + newBounds.width - 1, newBounds.top + newBounds.height - 1)) {
            shape.move(movement);
        }
    }
    else if (!isAlive) {
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            isAlive = true;
            isInvincible = true; // Activate invincibility
            invincibilityTimer = INVINCIBILITY_DURATION;
        }

        shape.setFillColor(Color(155, 155, 160));
        shape.setOutlineColor(Color::Blue);
        shape.setOutlineThickness(3);
    }
}


bool Player::isPlayerInvincible() const {
    return isInvincible;
}

void Player::activateInvincibility() {
    isInvincible = true;
    invincibilityTimer = INVINCIBILITY_DURATION;
    cout << "Invincibility activated!\n";
}



bool Player::checkForEnemies(vector<shared_ptr<Enemy>>& enemies, vector<shared_ptr<Enemy>>& BTenemies) {
    enemyNear = false;  // Reset flag
    enemyListPtr.clear();
    enemyListPtr.push_back(&enemies);
    enemyListPtr.push_back(&BTenemies);

    for (auto& enemyList : enemyListPtr) {
        for (auto& enemy : *enemyList) {  // Iterate using unique_ptr
            if (enemy->isStunned()) { continue; }
            float distance = sqrt(
                pow(enemy->shape.getPosition().x - shape.getPosition().x, 2) +
                pow(enemy->shape.getPosition().y - shape.getPosition().y, 2)
            );

            if (distance <= DETECTION_RADIUS) {
                enemyNear = true;
                cout << "Enemy detected!\n";
                return enemyNear;  // Stop checking after detecting one enemy
            }
        }
    }

    return enemyNear;
}



void Player::setIsAlive(bool alive) {
	isAlive = alive;
}

Vector2f Player::getNearestEnemyPosition() {
    if (enemyListPtr.empty()) {
        cout << "Error: No enemy list available!\n";
        return Vector2f(0.f, 0.f);
    }

    Vector2f nearestEnemyPos;
    float minDistance = numeric_limits<float>::max();

    for (auto& enemyList : enemyListPtr) {
        for (const auto& enemy : *enemyList) {  // Iterate properly with unique_ptr
            if (enemy->isStunned()) { continue; }
            float distance = sqrt(
                pow(enemy->shape.getPosition().x - shape.getPosition().x, 2) +
                pow(enemy->shape.getPosition().y - shape.getPosition().y, 2)
            );

            if (distance < minDistance) {
                minDistance = distance;
                nearestEnemyPos = enemy->shape.getPosition();
            }
        }
    }

    return nearestEnemyPos;
}





Vector2f Player::getPosition() {
    return shape.getPosition();
}

bool Player::getisAlive() {
    return isAlive;
}

bool Player::getIsEnemyNear() {
    return enemyNear;
}


Player player(200, 400);


