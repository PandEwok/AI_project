#ifndef ALLY_HPP
#define ALLY_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include <vector>
#include "Enemy.hpp"

class Ally : public Entity {
private:
    bool closePlayer = false;
    float reviveProgress = 0.0f;  // Time spent reviving
    bool isReviving = false;

public:
    static constexpr float SPEED = 100.0f;
    static constexpr float REVIVE_TIME = 3.0f;  // 3 seconds to revive
    float deltaTime = 0.0f;

    Ally(float x, float y);
    void update(float deltaTime, Grid& grid) override;
    bool getClosePlayer();
    void setClosePlayer(bool closePlayer);
    void startReviving();
    void stopReviving();
    bool isRevivalComplete();
    void resetReviveProgress();
	void setReviveProgress(float reviveProgress);
	float getReviveProgress();
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

class RevivePlayerAction : public Action {
private:
    Ally* ally;
public:
    RevivePlayerAction(Ally* ally);
    bool CanExecute() override;
    void Execute() override;
};

class DefendPlayerAction : public Action {
private:
    Ally* ally;
public:
    DefendPlayerAction(Ally* ally);
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
