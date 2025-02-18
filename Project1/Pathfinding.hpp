#pragma once

#include "Global.hpp"
#include "Grid.hpp"
#include "Node.hpp"

class Pathfinding {
public:
    static std::vector<sf::Vector2i> findPath(Grid& grid, sf::Vector2i start, sf::Vector2i end);
};