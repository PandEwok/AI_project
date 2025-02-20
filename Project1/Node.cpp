#include "Node.hpp"

Node::Node(Vector2i pos) : position(pos), gCost(0), hCost(0), fCost(0), parent(nullptr) {}

void Node::calculateCosts(Node* end, int newG) {
    gCost = newG;
    hCost = sqrt(pow(position.x - end->position.x, 2) + pow(position.y - end->position.y, 2));
    fCost = (float)gCost + hCost;
}