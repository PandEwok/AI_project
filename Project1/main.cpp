#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include <vector>
#include "Ally.hpp"
#include "BigEnemy.hpp"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);


    std::vector<Ally> allies = { Ally(100, 100)};
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(700, 100));    // Normal enemy
    enemies.push_back(std::make_unique<BigEnemy>(600, 300)); // Big enemy
    Grid grid;
    grid.loadFromFile("map.txt");

    // Create a GOAPAgent for each Ally and store them in a vector
    std::vector<GOAPAgent> allyAgents;
    for (auto& ally : allies) {
        allyAgents.emplace_back(&ally); // Pass each ally to its GOAPAgent
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(deltaTime, grid);
        player.checkForEnemies(enemies);  // Check if enemies are nearby
        for (auto& enemy : enemies) {
            BigEnemy* bigEnemy = dynamic_cast<BigEnemy*>(enemy.get());
            if (bigEnemy) {
                bigEnemy->update(deltaTime, grid, allies);  // Pass allies to BigEnemy
            }
            else {
                enemy->update(deltaTime, grid);  // Normal enemies update as usual
            }
        }

        for (auto& ally : allies) {
            ally.update(deltaTime, grid);
        }

        // Execute GOAP behaviors for each ally
        for (auto& ally : allies) {
            GOAPAgent agent(&ally);  // Pass the reference to the Ally
            agent.PerformActions();
        }




        window.clear();
        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : enemies)
            window.draw(enemy->shape);
        for (const auto& ally : allies)
            window.draw(ally.shape);
        window.display();

    }
    return 0;
}

