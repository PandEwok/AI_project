#include "Global.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "BTEnemy.hpp"
#include "Grid.hpp"
#include "Pathfinding.hpp"
#include <vector>
#include "Ally.hpp"
#include "BigEnemy.hpp"


int main() {
    window.setFramerateLimit(60);

    vector<shared_ptr<Enemy>> enemies;
    enemies.push_back(make_shared<BigEnemy>(600, 300)); // Big enemy
    vector<shared_ptr<Enemy>> BTenemies;
    BTenemies.push_back(make_shared<BTEnemy>(700, 100));

    grid.loadFromFile("map.txt");

    // Create a GOAPAgent for each Ally and store them in a vector
    vector<GOAPAgent> allyAgents;
    for (auto& ally : allies) {
        allyAgents.emplace_back(&ally); // Pass each ally to its GOAPAgent
    }

    Vector2i pos1 = Vector2i(-1, -1);
    Vector2i pos2 = Vector2i(-1, -1);

    Clock clock;

    while (window.isOpen()) {
        Time dt = clock.restart();
        deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed) {
                if (Mouse::isButtonPressed(Mouse::Right)) {
                    if (pos1 == Vector2i(-1, -1)) {
                        pos1 = Mouse::getPosition(window) / CELL_SIZE;
                        if (grid.getCell(pos1.x, pos1.y).walkable) {
                            grid.getCell(pos1.x, pos1.y).shape.setFillColor(Color::Blue - Color(0, 0, 0, 100));
                        }
                        else {
                            pos1 = Vector2i(-1, -1);
                        }
                    }
                    else if (pos2 == Vector2i(-1, -1)) {
                        pos2 = Mouse::getPosition(window) / CELL_SIZE;
                        if (grid.getCell(pos2.x, pos2.y).walkable) {
                            grid.getCell(pos2.x, pos2.y).shape.setFillColor(Color::Red - Color(0, 0, 0, 100));
                            cout << pos1.x << " " << pos1.y << " ; " << pos2.x << " " << pos2.y << endl;
                            for (Vector2i pos : Pathfinding::findPath(grid, pos1, pos2)) {
                                if (pos != pos1 and pos != pos2) {
                                    grid.getCell(pos.x, pos.y).shape.setFillColor(Color::Green - Color(0, 0, 0, 100));
                                }
                            }
                        }
                        else {
                            pos2 = Vector2i(-1, -1);
                        }
                    }
                    else {
                        for (Vector2i pos : Pathfinding::findPath(grid, pos1, pos2)) {
                            grid.getCell(pos.x, pos.y).shape.setFillColor(Color::Black);
                        }
                        pos1 = Vector2i(-1, -1);
                        pos2 = Vector2i(-1, -1);
                    }
                }
            }
        }

        player.update(deltaTime, grid);
        player.checkForEnemies(enemies, BTenemies);  // Check if enemies are nearby

        for (auto& enemy : enemies) {
            BigEnemy* bigEnemy = dynamic_cast<BigEnemy*>(enemy.get());
            if (bigEnemy) {
                bigEnemy->update(deltaTime, grid);  // Pass allies to BigEnemy
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

        for (auto& enemy : BTenemies) {
            enemy->update(deltaTime, grid);
        }

        window.clear();
        grid.draw(window);

        for (const auto& enemy : enemies) {
            window.draw(enemy->shape);
            RectangleShape rect(Vector2f(1, 1));
            rect.setPosition(enemy->shape.getPosition());
            window.draw(rect);
        }
        for (const auto& enemy : BTenemies) {
            for (Vector2i pos : enemy->waypoints) {
                RectangleShape rect(Vector2f(CELL_SIZE, CELL_SIZE));
                rect.setOrigin(CELL_SIZE / 2, CELL_SIZE / 2);
                rect.setPosition(Vector2f(pos));
                rect.setFillColor(Color(255, 20, 20, 100));
                window.draw(rect);
            }
            window.draw(enemy->shape);
            RectangleShape rect(Vector2f(1, 1));
            rect.setPosition(enemy->shape.getPosition());
            window.draw(rect);
        }
        window.draw(player.shape);
        for (const auto& ally : allies)
            window.draw(ally.shape);
        window.display();

    }
    return 0;
}

