#include "BTNode.hpp"
#include "Pathfinding.hpp"

void SequenceNode::AddChild(unique_ptr<BTNode> child) {
    children.push_back(move(child));
}

NodeState SequenceNode::execute() {
    for (auto& child : children) {
        if (child->execute() == NodeState::FAILURE) {
            return NodeState::FAILURE;
        }
    }
    return NodeState::SUCCESS;
}

void SelectorNode::AddChild(unique_ptr<BTNode> child) {
    children.push_back(move(child));
}

NodeState SelectorNode::execute() {
    for (auto& child : children) {
        if (child->execute() == NodeState::SUCCESS) {
            return NodeState::SUCCESS;
        }
    }
    return NodeState::FAILURE;
}

void Blackboard::SetValue(const string& key, int value) {
    data[key] = value;
}

int Blackboard::GetValue(const string& key) {
    return data[key];
}

NodeState ConditionNode::execute() {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

NodeState ActionNode::execute() {
    cout << "Action: " << actionName << endl;
    return NodeState::SUCCESS;
}

FollowActionNode::FollowActionNode(Entity& _self, Entity& _target) : self(_self), target(_target), ActionNode("Follow") {}

NodeState FollowActionNode::execute() {

    self.waypoints.clear();
    for (Vector2i pos : Pathfinding::findPath(grid, Vector2i(self.shape.getPosition()) / CELL_SIZE, Vector2i(target.shape.getPosition()) / CELL_SIZE)) {
        self.waypoints.push_back(pos * CELL_SIZE + Vector2i(CELL_SIZE / 2, CELL_SIZE / 2));
    }

    if (self.currentWaypoint >= self.waypoints.size()) {
        self.currentWaypoint = self.waypoints.size() - 1;
    }

    Vector2i goingTo = self.waypoints[self.currentWaypoint];
    Vector2f direction = Vector2f(goingTo) - self.shape.getPosition();
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = normalize(direction);

    if (distance < 5.0f) {
        if (self.currentWaypoint >= self.waypoints.size() - 1) {
            self.waypointChange = -1;
        }
        else if (self.currentWaypoint <= 0) {
            self.waypointChange = 1;
        }
        self.currentWaypoint += self.waypointChange;
    }
    else {
        self.shape.move(direction * self.SPEED * deltaTime);
    }
    cout << "Action: follow" << endl;
    return NodeState::SUCCESS;
}
