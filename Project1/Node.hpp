#pragma once

#include "Global.hpp"

struct Node {
    Vector2i position;
    float gCost, hCost, fCost;
    Node* parent;

    Node(Vector2i pos);
    void calculateCosts(Node* end, int newG);
};