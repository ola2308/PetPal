#include "Puppy.h"

void Puppy::setHappyTexture(const std::string& path) {
    if (!textureHappy.loadFromFile(path)) {
        std::cerr << "Nie udalo sie zaladowac tekstury " << path << std::endl;
    }
    if (isHappy && !isSleeping) {
        sprite.setTexture(textureHappy);
    }
}

void Puppy::setSadTexture(const std::string& path) {
    if (!textureSad.loadFromFile(path)) {
        std::cerr << "Nie udalo sie zaladowac tekstury " << path << std::endl;
    }
    if (!isHappy && !isSleeping) {
        sprite.setTexture(textureSad);
    }
}

void Puppy::setSleepTexture(const std::string& path) {
    if (!textureSleep.loadFromFile(path)) {
        std::cerr << "Nie udalo sie zaladowac spiacej tekstury " << path << std::endl;
    }
    if (isSleeping) {
        sprite.setTexture(textureSleep);
    }
}

void Puppy::updateTexture(bool isHappy) {
    if (isSleeping) {
        sprite.setTexture(textureSleep);
    }
    else if (isHappy) {
        sprite.setTexture(textureHappy);
    }
    else {
        sprite.setTexture(textureSad);
    }
    this->isHappy = isHappy;
}

void Puppy::display(sf::RenderWindow& window) {
    window.draw(sprite);
}
