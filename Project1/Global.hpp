#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;
using namespace sf;


extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern RenderWindow window;

extern float deltaTime;

Vector2f normalize(Vector2f value);