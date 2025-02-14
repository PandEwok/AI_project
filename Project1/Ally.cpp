#include "Ally.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

Ally::Ally(float x, float y) : Entity(x, y, sf::Color::Yellow) {}

void Ally::update(float deltaTime, Grid& grid) {

}

class State {
public:
    bool hasFood = false;  // La nourriture est-elle disponible ?
    int hunger = 100;      // Le niveau de faim (100 = faim maximale)

    bool HasFood() const { return hasFood; }
    int GetHunger() const { return hunger; }

    void SetFood(bool food) { hasFood = food; }
    void ReduceHunger() { hunger = std::max(0, hunger - 50); }  // Réduit la faim de 50 points
    void SetHunger(int level) { hunger = level; }
};

class Action {
public:
    virtual bool CanExecute(const State& state) = 0;
    virtual void Execute(State& state) = 0;
    virtual ~Action() {}
};

class EatAction : public Action {
public:
    bool CanExecute(const State& state) override {
        return state.HasFood() && state.GetHunger() > 0;
    }

    void Execute(State& state) override {
        std::cout << "L'agent mange.\n";
        state.ReduceHunger();  // Réduit la faim après avoir mangé
        state.SetFood(false);   // Après avoir mangé, il n'y a plus de nourriture
    }
};

class SearchFoodAction : public Action {
public:
    bool CanExecute(const State& state) override {
        return !state.HasFood();  // Peut chercher de la nourriture si l'agent n'en a pas
    }

    void Execute(State& state) override {
        std::cout << "L'agent cherche de la nourriture.\n";
        state.SetFood(true);  // Trouve de la nourriture
    }
};

enum class Goal {
    Manger,
    ChercherNourriture
};


class GOAPPlanner {
public:
    std::vector<Action*> Plan(const State& initialState, Goal goal) {
        std::vector<Action*> plan;

        if (goal == Goal::Manger) {
            if (initialState.GetHunger() > 0 && !initialState.HasFood()) {
                plan.push_back(new SearchFoodAction());
                plan.push_back(new EatAction());
            }
            else if (initialState.HasFood()) {
                plan.push_back(new EatAction());
            }
        }

        return plan;
    }
};

class GOAPAgent {
private:
    State state;
    GOAPPlanner planner;

public:
    GOAPAgent() {
        state.SetHunger(100);  // Initialement, l'agent a faim
    }

    void PerformActions() {
        Goal goal = Goal::Manger;  // L'objectif de l'agent est de manger

        std::vector<Action*> plan = planner.Plan(state, goal);

        for (auto action : plan) {
            if (action->CanExecute(state)) {
                action->Execute(state);  // Exécute l'action
            }
            else {
                std::cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Libérer la mémoire
        }
    }

    void PrintState() {
        std::cout << "Faim: " << state.GetHunger() << "\n";
        std::cout << "Nourriture disponible: " << (state.HasFood() ? "Oui" : "Non") << "\n";
    }
};