#include "Ally.hpp"
#include <cmath>
#include <iostream>

Ally::Ally(float x, float y) : Entity(x, y, Color::Yellow) {}
int Ally::alliesReviving = 0;

void Ally::increaseRevivalCount() {
    alliesReviving++;
}
void Ally::decreaseRevivalCount() {
    if (alliesReviving > 0)
        alliesReviving--;
}


void Ally::update(float deltaTime, Grid& grid) {
    this->deltaTime = deltaTime;
    applySeparationForce(allies);  // ✅ Keep allies spaced out
}


bool Ally::getClosePlayer() {
	return closePlayer;
}

void Ally::setClosePlayer(bool closePlayer) {
	this->closePlayer = closePlayer;
}

void Ally::stopReviving() {
    isVulnerable = false;
    shape.setOutlineThickness(0);
}

bool Ally::isRevivalComplete() {
    return reviveProgress >= REVIVE_TIME;
}

void Ally::resetReviveProgress() {
    reviveProgress = 0.0f;
}

void Ally::setReviveProgress(float reviveProgress) {
	this->reviveProgress = reviveProgress;
}

float Ally::getReviveProgress() {
	return reviveProgress;
}

bool Ally::getisVulnerable() {
	return isVulnerable;
}

void Ally::setisVulnerable(bool vulnerable) {
    isVulnerable = vulnerable;
}

bool Ally::isAllyAlive() {
	return isAlive;
}

void Ally::setAllyAlive(bool alive) {
	isAlive = alive;
}


void Ally::applySeparationForce(vector<Ally>& allies) {
    const float SEPARATION_DISTANCE = 35.0f;  // Minimum spacing
    const float PUSH_FORCE = 0.5f;            // Strength of push

    Vector2f totalPush(0, 0);
    int count = 0;

    auto normalize = [](const Vector2f& vec) -> Vector2f {
        float length = sqrt(vec.x * vec.x + vec.y * vec.y);
        return (length > 0) ? Vector2f(vec.x / length, vec.y / length) : Vector2f(0, 0);
        };

    for (Ally& other : allies) {
        if (&other == this) continue;  // Skip self

        Vector2f diff = shape.getPosition() - other.shape.getPosition();
        float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance < SEPARATION_DISTANCE && distance > 0) {
            Vector2f normalizedDiff = normalize(diff);
            totalPush += normalizedDiff; // Accumulate repelling force
            count++;
        }
    }

    if (count > 0) {
        totalPush /= static_cast<float>(count);  // Get average push
        shape.move(totalPush * PUSH_FORCE);  // Apply push
    }
}



// ChasePlayerAction
ChasePlayerAction::ChasePlayerAction(Ally* ally) : ally(ally) {}

bool ChasePlayerAction::CanExecute() {
    return 1;
}

void ChasePlayerAction::Execute() {
    if (!ally) {
        cout << "Error: ChasePlayerAction has a null ally reference!\n";
        return;
    }

    Vector2f playerPos = player.getPosition();
    Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    const float MIN_DISTANCE = 20.0f;  // Minimum distance from Player
    const float MAX_DISTANCE = 50.0f; // Max distance before moving in

    float distance = sqrt(pow(playerPos.x - allyPos.x, 2) + pow(playerPos.y - allyPos.y, 2));

    if (distance > MAX_DISTANCE) {
        // Too far? Move closer.
        ally->setClosePlayer(false);
        Vector2f direction = playerPos - allyPos;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
    }
    else if (distance < MIN_DISTANCE) {
		ally->setClosePlayer(true);
        // Too close? Push away from the Player.
        Vector2f direction = allyPos - playerPos; // Reverse direction
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        // After moving:
        ally->shape.move(direction * Ally::SPEED * deltaTime);
        ally->applySeparationForce(allies);  // ✅ Prevent overlapping

    }
}



RevivePlayerAction::RevivePlayerAction(Ally* ally) : ally(ally) {}

bool RevivePlayerAction::CanExecute() {
    return !player.getisAlive(); // Only execute if the player is dead and no one else is reviving
}

void RevivePlayerAction::Execute() {
    if (!ally) {
        cout << "Error: RevivePlayerAction has a null ally reference!\n";
        return;
    }

    if (!ally->isAllyAlive()) {
        // If this ally dies while reviving, decrement count
        if (Ally::alliesReviving > 0) {
            Ally::decreaseRevivalCount();
        }
        return;
    }

    Vector2f playerPos = player.getPosition();
    Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    float distance = sqrt(pow(playerPos.x - allyPos.x, 2) + pow(playerPos.y - allyPos.y, 2));
    const float MIN_DISTANCE = 45.0f;

    ally->setisVulnerable(true);

    if (distance > MIN_DISTANCE) {
        Vector2f direction = playerPos - allyPos;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        ally->shape.move(direction * Ally::SPEED * deltaTime);
        ally->applySeparationForce(allies);

        return;
    }

    // If no valid allies are reviving, start revival
    if (Ally::alliesReviving == 0) {
        Ally::increaseRevivalCount();
    }

    ally->setClosePlayer(true);
    ally->setReviveProgress(ally->getReviveProgress() + deltaTime);

    float progress = ally->getReviveProgress();
    if (static_cast<int>(progress * 5) % 2 == 0) {
        ally->shape.setOutlineThickness(3);
        ally->shape.setOutlineColor(Color::Black);
    }
    else {
        ally->shape.setFillColor(Color::Yellow);
        ally->shape.setOutlineThickness(3);
        ally->shape.setOutlineColor(Color::White);
    }

    if (ally->isRevivalComplete()) {
        cout << "Player revived!\n";

        player.setIsAlive(true);
        player.activateInvincibility();
        ally->resetReviveProgress();
        ally->setisVulnerable(false);
        ally->shape.setOutlineThickness(0);
        ally->shape.setFillColor(Color::Yellow);

        Ally::decreaseRevivalCount();
    }
}



// DefendPlayerAction
DefendPlayerAction::DefendPlayerAction(Ally* ally) : ally(ally) {}

bool DefendPlayerAction::CanExecute() {
    return player.getIsEnemyNear();
}

void DefendPlayerAction::Execute() {
    if (!ally) {
        cout << "Error: DefendPlayerAction has a null ally reference!\n";
        return;
    }

    Vector2f playerPos = player.getPosition();
    Vector2f enemyPos = player.getNearestEnemyPosition();
    Vector2f allyPos = ally->shape.getPosition();
    float deltaTime = ally->deltaTime;

    if (enemyPos == Vector2f(0.f, 0.f)) {
        cout << "No enemy found, cannot defend!\n";
        return;
    }

    Vector2f defendPosition = playerPos + (enemyPos - playerPos) * 0.35f;

    const float MIN_DISTANCE = 20.0f;
    const float MAX_DISTANCE = 20.0f;

    Vector2f direction = defendPosition - allyPos;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > MAX_DISTANCE) {
        // Move closer
        direction /= length;
        // After moving:
        ally->shape.move(direction * Ally::SPEED * deltaTime);
        ally->applySeparationForce(allies);  // ✅ Prevent overlapping

    }
    else if (length < MIN_DISTANCE) {
        // Move away slightly
        direction = allyPos - playerPos;
        length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) direction /= length;

        // After moving:
        ally->shape.move(direction * Ally::SPEED * deltaTime);
        ally->applySeparationForce(allies);  // ✅ Prevent overlapping

    }
}

DeathAction::DeathAction(Ally* ally) : ally(ally) {}
bool DeathAction::CanExecute(){
    return !ally->isAllyAlive(); // Execute only if the ally is dead
}

void DeathAction::Execute() {
    cout << "Ally has died. No further actions can be taken.\n";
    ally->shape.setFillColor(Color(130, 175, 130)); 
    ally->shape.setOutlineThickness(0);

    // Ensure dead allies don't count as revivers
    if (Ally::alliesReviving > 0) {
        Ally::decreaseRevivalCount();
    }
}






// GOAPPlanner
GOAPPlanner::GOAPPlanner() {}

vector<Action*> GOAPPlanner::Plan(Goal goal, Ally* ally) {
    vector<Action*> plan;

    if (goal == Goal::Revive) {
        plan.push_back(new RevivePlayerAction(ally));
    }
    else if (goal == Goal::Chase) {
        plan.push_back(new ChasePlayerAction(ally));
    }
    else if (goal == Goal::Defend) {
        plan.push_back(new DefendPlayerAction(ally));
    }
	else if (goal == Goal::Death) {
		plan.push_back(new DeathAction(ally));
	}
    else {
        cout << "Error: Unknown goal\n";
    }
    return plan;
}

// GOAPAgent
GOAPAgent::GOAPAgent(Ally* ally) : owner(ally), goal(Goal::Chase) {
    UpdatePlan();
}

void GOAPAgent::UpdateGoal(Goal newGoal) {
    if (goal != newGoal) {
        goal = newGoal;
        UpdatePlan();
    }
}

void GOAPAgent::UpdatePlan() {
    plan = planner.Plan(goal, owner);
}

void GOAPAgent::PerformActions() {
    cout << "Current Goal: " << static_cast<int>(goal) << endl;

    if (!owner->isAllyAlive()) {
        cout << "Switching to Death goal\n";
        UpdateGoal(Goal::Death);
    }
    else if (owner->getReviveProgress() > 0) {  // ✅ If already reviving, don't change goal
        UpdateGoal(Goal::Revive);
    }
    else if (!player.getisAlive()) {
        if (Ally::alliesReviving == 0) {
            UpdateGoal(Goal::Revive);
        }
        else {
            cout << "One or more allies reviving, but this one is NOT the reviver.\n";
            owner->setisVulnerable(false);
            if (player.getIsEnemyNear()) {
                UpdateGoal(Goal::Defend);
            }
            else {
                UpdateGoal(Goal::Chase);
            }
        }
    }
    else if (player.getIsEnemyNear()) {
        cout << "Switching to Defend goal (Enemy near and player alive)\n";
        UpdateGoal(Goal::Defend);
    }
    else {
        cout << "Switching to Chase goal\n";
        UpdateGoal(Goal::Chase);
    }

    cout << "New Goal After Update: " << static_cast<int>(goal) << endl;

    for (auto& action : plan) {
        if (action->CanExecute()) {
            action->Execute();
        }
        else {
            cout << "Action impossible: " << typeid(*action).name() << "\n";
        }
    }
}



vector<Action*> GOAPAgent::getPlan() {
    return plan;
}


vector<Ally> allies = { Ally(100, 100) , Ally(150,150)};