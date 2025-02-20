#pragma once

#include "Enemy.hpp"
#include "Pathfinding.hpp"

class BigEnemy : public Enemy {
private:
    static constexpr float DETECTION_RADIUS = 255.0f;

public:
    BigEnemy(float x, float y);

    // BigEnemy extends update to check for allies
    void update(float deltaTime, Grid& grid);
};
