#pragma once

#include "Global.hpp"
#include "Grid.hpp"
#include "Node.hpp"

class Pathfinding {
public:
    static vector<Vector2i> findPath(Grid& grid, Vector2i start, Vector2i end);
};