#ifndef ALLY_HPP
#define ALLY_HPP

#include "Entity.hpp"

class Ally : public Entity {
public:
    static constexpr float SPEED = 100.0f;
    Ally(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};

#endif // Ally_HPP