#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity {
public:
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};

extern Player player;

#endif // PLAYER_HPP