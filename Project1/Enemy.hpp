#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Ally.hpp"
#include "Player.hpp"  // Added this include

class Ally;
class Player;  // Forward declaration in case of circular dependencies

class Enemy : public Entity {
protected:
    float stunTimer = 0.0f;
    static constexpr float STUN_DURATION = 3.0f;
    static constexpr float DETECTION_RADIUS = 40 * 6;

public:
    Enemy(float x, float y);

    void update(float deltaTime, Grid& grid) override;

    void checkForAllyCollision(vector<Ally>& allies);
    void checkForPlayerCollision(Player& player);

    bool isStunned() const;

    sf::Vector2f getPosition();
};

#endif // ENEMY_HPP
