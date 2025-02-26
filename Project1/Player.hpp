#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Enemy.hpp"
#include <vector>
#include <iostream>

class Enemy;

class Player : public Entity {
private:
    vector<vector<shared_ptr<Enemy>>*> enemyListPtr = {};  // Store pointer to unique_ptr vector


    bool isInvincible = false;
    float invincibilityTimer = 0.0f;
    static constexpr float INVINCIBILITY_DURATION = 2.0f; // 2 seconds of invincibility
    bool isAlive = true;
    bool enemyNear = false;
    static constexpr float DETECTION_RADIUS = 250.0f;

public:
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
    Vector2f getPosition();
    bool getisAlive();
	void setIsAlive(bool alive);
    bool getIsEnemyNear();

    void activateInvincibility();
    bool isPlayerInvincible() const;

    bool checkForEnemies(vector<shared_ptr<Enemy>>& enemies, vector<shared_ptr<Enemy>>& BTenemies);

    Vector2f getNearestEnemyPosition();


};

extern Player player;

#endif // PLAYER_HPP