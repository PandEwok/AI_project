#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(float x, float y) : Entity(x, y, sf::Color::Blue) { SPEED = 250.0f; }

void Player::update(float deltaTime, Grid& grid) {

    if (isAlive) {
        shape.setFillColor(sf::Color::Blue);
		shape.setOutlineThickness(0);

        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) movement.y -= SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) movement.x -= SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += SPEED * deltaTime;
        //DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) isAlive = !isAlive;

    sf::Vector2f newPosition = shape.getPosition() - shape.getSize()/2.f + movement;
    sf::FloatRect newBounds(newPosition, shape.getSize());

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
        //DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) isAlive = !isAlive;


        shape.setFillColor(sf::Color(155, 155, 160));
        shape.setOutlineColor(sf::Color::Blue);
        shape.setOutlineThickness(3);
    }
}

void Player::checkForEnemies(const std::vector<Enemy>& enemies) {
    enemyNear = false;  // Reset flag
    enemyList = &enemies;  // Store reference to enemy list

    for (const Enemy& enemy : enemies) {
        float distance = std::sqrt(std::pow(enemy.shape.getPosition().x - shape.getPosition().x, 2) +
            std::pow(enemy.shape.getPosition().y - shape.getPosition().y, 2));
        if (distance <= DETECTION_RADIUS) {
            enemyNear = true;
            std::cout << "Enemy detected!\n";
            return;  // Stop checking after detecting one enemy
        }
    }
}

void Player::setIsAlive(bool alive) {
	isAlive = alive;
}

sf::Vector2f Player::getNearestEnemyPosition() {
    if (!enemyList) {
        std::cout << "Error: No enemy list available!\n";
        return sf::Vector2f(0.f, 0.f);
    }

    sf::Vector2f nearestEnemyPos;
    float minDistance = std::numeric_limits<float>::max();

    for (const Enemy& enemy : *enemyList) {
        float distance = std::sqrt(std::pow(enemy.shape.getPosition().x - shape.getPosition().x, 2) +
            std::pow(enemy.shape.getPosition().y - shape.getPosition().y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestEnemyPos = enemy.shape.getPosition();
        }
    }

    return nearestEnemyPos;
}




sf::Vector2f Player::getPosition() {
    return shape.getPosition();
}

bool Player::getisAlive() {
    return isAlive;
}

bool Player::getIsEnemyNear() {
    return enemyNear;
}

Player player(200, 400);
