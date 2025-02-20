#include "Entity.hpp"

Entity::Entity(float x, float y, sf::Color color) {
    shape.setSize({ 35, 35 });
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(x, y);
    shape.setFillColor(color);
}
