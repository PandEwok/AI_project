#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include <iostream>



class Player : public Entity {
private:
    bool isAlive = true;
	bool EnemyNear = false;

public:
    static constexpr float SPEED = 200.0f;
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
    sf::Vector2f getPosition();
	bool getisAlive();
};

extern Player player;

#endif // PLAYER_HPP