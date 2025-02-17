#ifndef ALLY_HPP
#define ALLY_HPP

#include "Entity.hpp"
#include "Player.hpp"

class Ally : public Entity {
public:
    static constexpr float SPEED = 125.0f;
    float deltaTime = 0.0f;  // Store the latest deltaTime

    Ally(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};




class Action {
public:
    virtual bool CanExecute() = 0;
    virtual void Execute() = 0;
    virtual ~Action() {}
};

class ProtectPlayerAction : public Action {
private:
    Ally* ally;  // Pointer to the ally that will execute this action
public:
    ProtectPlayerAction(Ally* ally) : ally(ally) {}

    bool CanExecute() override;
    void Execute() override;
};


enum class Goal {
    Revive,
    Defend
};

class GOAPPlanner {
public:
    GOAPPlanner();
    std::vector<Action*> Plan(Goal& goal, Ally* ally);


};
class GOAPAgent {   
public:
    Goal goal = Goal::Defend;
    GOAPPlanner planner;
    Ally* owner;  // The Ally that owns this GOAPAgent
    std::vector<Action*> plan = planner.Plan(goal, owner);

    GOAPAgent(Ally* ally) : owner(ally) {
        plan = planner.Plan(goal, owner);  // Pass the Ally instance
    }

    void PerformActions();

    std::vector<Action*> getPlan();
};

#endif // Ally_HPP