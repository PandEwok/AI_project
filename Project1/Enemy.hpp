#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Ally.hpp"
#include "Player.hpp"  // Added this include
#include <vector>

class Ally;
class Player;  // Forward declaration in case of circular dependencies

class Enemy : public Entity {
protected:
    float stunTimer = 0.0f;
    static constexpr float STUN_DURATION = 3.0f;

public:
    Enemy(float x, float y);

    void update(float deltaTime, Grid& grid) override;

    void checkForAllyCollision(std::vector<Ally>& allies);
    void checkForPlayerCollision(Player& player);

    bool isStunned() const;
};

#endif // ENEMY_HPP
