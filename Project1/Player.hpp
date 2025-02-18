#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Enemy.hpp"
#include <vector>
#include <iostream>


class Player : public Entity {
private:
    const std::vector<Enemy>* enemyList = nullptr;  // Store reference to enemies
    bool isAlive = true;
    bool enemyNear = false;
    static constexpr float DETECTION_RADIUS = 250.0f;

public:
    static constexpr float SPEED = 200.0f;
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
    sf::Vector2f getPosition();
    bool getisAlive();
	void setIsAlive(bool alive);
    bool getIsEnemyNear();

    void checkForEnemies(const std::vector<Enemy>& enemies);
    sf::Vector2f getNearestEnemyPosition();

};

extern Player player;

#endif // PLAYER_HPP
