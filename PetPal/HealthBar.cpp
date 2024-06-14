#include "HealthBar.h"

HealthBar::HealthBar(const std::string& name, sf::Texture& texture, const sf::Vector2f& position)
    : name(name), iconTexture(texture), currentValue(100), maxValue(100), decreaseRate(1) {
    iconSprite.setTexture(iconTexture);
    iconSprite.setPosition(position);

    barBackground.setSize(sf::Vector2f(100, 20));
    barBackground.setFillColor(sf::Color::Red);
    barBackground.setPosition(position.x + 50, position.y);

    barForeground.setSize(sf::Vector2f(100, 20));
    barForeground.setFillColor(sf::Color::Green);
    barForeground.setPosition(position.x + 50, position.y);
}

void HealthBar::updateHealth(float deltaTime, sf::Sprite& puppySprite) {
    currentValue = std::max(0.0f, currentValue - decreaseRate * deltaTime);
    float percentage = currentValue / maxValue;
    barForeground.setSize(sf::Vector2f(100 * percentage, 20));
    barForeground.setFillColor(sf::Color(255 * (1 - percentage), 255 * percentage, 0));
}

void HealthBar::draw(sf::RenderWindow& window) const {
    window.draw(barBackground);
    window.draw(barForeground);
    window.draw(iconSprite);
}

float HealthBar::getCurrentValue() const {
    return currentValue;
}

float HealthBar::getMaxValue() const {
    return maxValue;
}

std::string HealthBar::getName() const {
    return name;
}

void HealthBar::setCurrentValue(float x) {
    currentValue = x;
}