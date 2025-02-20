#include "BigEnemy.hpp"

BigEnemy::BigEnemy(float x, float y) : Enemy(x, y) {
    shape.setSize(Vector2f(80, 80));
    shape.setOrigin(shape.getSize() / 2.f);
}

void BigEnemy::update(float deltaTime, Grid& grid) {
    Enemy::update(deltaTime, grid);  // Call base update (handles stun logic)
    checkForAllyCollision(allies);   // Ensure big enemies also get stunned

    if (isStunned()) return;  // Skip movement if stunned

    //UPDATE
    Vector2f playerPos = player.getPosition();
    Vector2f myPos = shape.getPosition();

    // Calculate distance to player
    float distance = sqrt(pow(playerPos.x - myPos.x, 2) + pow(playerPos.y - myPos.y, 2));

    if (distance < DETECTION_RADIUS) {  // If player is within range, move toward them
        Vector2f direction = playerPos - myPos;

        // Normalize the direction vector
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }

        // Move towards the player
        if (player.getisAlive()) {
            shape.move(direction * SPEED * deltaTime);
        }
    }

    // Normal enemy update logic (move, attack, etc.)

    checkForAllyCollision(allies);
    checkForPlayerCollision(player);
}