#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Grid.hpp"

class Entity {
public:
    RectangleShape shape;
    float SPEED = 180.0f;
    Vector2f velocity;
    vector<Vector2i> waypoints = {};
    int currentWaypoint = 0;
    int waypointChange = 1;

    Entity(float x, float y, Color color);
    virtual void update(float deltaTime, Grid& grid) = 0;
};

#endif // ENTITY_HPP