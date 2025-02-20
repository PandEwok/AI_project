#pragma once

#include "Global.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"

enum class NodeState { SUCCESS, FAILURE, RUNNING };

class BTNode {
public:
    virtual ~BTNode() = default;
    virtual NodeState execute() = 0;
};

class SequenceNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState execute() override;
};


class SelectorNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState execute() override;
};


class Blackboard {
private:
    std::unordered_map<std::string, int> data;
public:
    void SetValue(const std::string& key, int value);
    int GetValue(const std::string& key);
};


class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    int expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState execute() override;
};

class ActionNode : public BTNode {
private:
    std::string actionName;
public:
    ActionNode(std::string name) : actionName(name) {}
    NodeState execute() override;
};

class FollowActionNode : public ActionNode {
public:
    Entity& self;
    Entity& target;
    FollowActionNode(Entity& _self, Entity& _target);
    NodeState execute() override;
};