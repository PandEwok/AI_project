#ifndef BIGENEMY_HPP
#define BIGENEMY_HPP

#include "Enemy.hpp"
#include <vector>

class BigEnemy : public Enemy {
private:
    static constexpr float DETECTION_RADIUS = 255.0f;

public:
    BigEnemy(float x, float y);

    // BigEnemy extends update to check for allies
    void update(float deltaTime, Grid& grid, std::vector<Ally>& allies);
};

#endif
