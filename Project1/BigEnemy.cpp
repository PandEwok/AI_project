#include "BigEnemy.hpp"

BigEnemy::BigEnemy(float x, float y) : Enemy(x, y) {
    shape.setSize(Vector2f(80, 80));
    shape.setOrigin(shape.getSize() / 2.f);
}

void BigEnemy::update(float deltaTime, Grid& grid) {
    Enemy::update(deltaTime, grid);  // Call base update (handles stun logic)
    waypoints.clear();
    checkForAllyCollision(allies);   // Ensure big enemies also get stunned

    if (isStunned()) return;  // Skip movement if stunned

    //UPDATE
    Vector2f playerPos = player.getPosition();
    Vector2f myPos = shape.getPosition();
    
    Vector2f direction = playerPos - myPos;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < DETECTION_RADIUS) {
        // Move towards the player
        if (player.getisAlive()) {
            for (Vector2i pos : Pathfinding::findPath(grid, Vector2i(shape.getPosition()) / CELL_SIZE, Vector2i(player.shape.getPosition()) / CELL_SIZE)) {
                waypoints.push_back(pos * CELL_SIZE + Vector2i(CELL_SIZE / 2, CELL_SIZE / 2));
            }

            if (currentWaypoint >= waypoints.size()) {
                currentWaypoint = waypoints.size() - 1;
            }

            Vector2i goingTo = waypoints[currentWaypoint];
            Vector2f direction = Vector2f(goingTo) - shape.getPosition();
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            direction = normalize(direction);
            if (distance < 5.f) {
                if (currentWaypoint >= waypoints.size() - 1) {
                    waypointChange = -1;
                }
                else if (currentWaypoint <= 0) {
                    waypointChange = 1;
                }
                currentWaypoint += waypointChange;
            }
            else {
                shape.move(direction * SPEED * deltaTime);
            }
        }
    }

    // Normal enemy update logic (move, attack, etc.)

    checkForAllyCollision(allies);
    checkForPlayerCollision(player);
}