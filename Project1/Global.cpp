#include "Global.hpp"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");

float deltaTime;

Vector2f normalize(Vector2f value)
{
    float baseX = 1.f;
    float baseY = 1.f;
    if (int(value.x) < 0) {
        baseX = -1.f;
    }
    else if (int(value.x) == 0) {
        baseX = 0;
    }
    if (int(value.y) < 0) {
        baseY = -1.f;
    }
    else if (int(value.y) == 0) {
        baseY = 0;
    }
    if (abs(value.x) > abs(value.y)) {
        return Vector2f(baseX, baseY - baseY * abs(value.y / value.x));
    }
    else if (abs(value.x) < abs(value.y)) {
        return Vector2f(baseX - baseX * abs(value.x / value.y), baseY);
    }
    else {
        return Vector2f(baseX, baseY);
    }
}