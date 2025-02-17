#ifndef ALLY_HPP
#define ALLY_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include <vector>

class Ally : public Entity {
public:
    static constexpr float SPEED = 125.0f;
    float deltaTime = 0.0f;

    Ally(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};

class Action {
public:
    virtual bool CanExecute() = 0;
    virtual void Execute() = 0;
    virtual ~Action() {}
};

class ChasePlayerAction : public Action {
private:
    Ally* ally;
public:
    ChasePlayerAction(Ally* ally);
    bool CanExecute() override;
    void Execute() override;
};

enum class Goal {
    Chase,
    Revive,
    Defend
};

class GOAPPlanner {
public:
    GOAPPlanner();
    std::vector<Action*> Plan(Goal goal, Ally* ally);
};

class GOAPAgent {
public:
    Goal goal;
    GOAPPlanner planner;
    Ally* owner;
    std::vector<Action*> plan;

    GOAPAgent(Ally* ally);

    void UpdateGoal(Goal newGoal);
    void UpdatePlan();
    void PerformActions();
    std::vector<Action*> getPlan();
};

#endif // ALLY_HPP
