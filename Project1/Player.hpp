#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Enemy.hpp"
#include <vector>
#include <iostream>


class Player : public Entity {
private:
    bool isAlive = true;
    bool enemyNear = false;
    static constexpr float DETECTION_RADIUS = 200.0f;

public:
    static constexpr float SPEED = 200.0f;
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
    sf::Vector2f getPosition();
    bool getisAlive();
    bool getIsEnemyNear();

    void checkForEnemies(const std::vector<Enemy>& enemies);  // New function
};

extern Player player;

#endif // PLAYER_HPP
