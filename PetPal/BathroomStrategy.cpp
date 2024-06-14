#include "BathroomStrategy.h"

//Konstruktor 
BathroomStrategy::BathroomStrategy(sf::RenderWindow& win, RoomInteraction& roomInter)
    : window(win), roomInteraction(roomInter) {
    if (!soapTexture.loadFromFile("reszta/mydlo.png")) {
        std::cerr << "Nie mozna zaladowac tekstury mydla: Mydlo.png" << std::endl;
        exit(1);
    }
    soapSprite.setTexture(soapTexture);
    originalSoapPosition = sf::Vector2f(window.getSize().x - soapSprite.getGlobalBounds().width, window.getSize().y - soapSprite.getGlobalBounds().height);
    soapSprite.setPosition(originalSoapPosition);

    if (!bubbleTexture.loadFromFile("reszta/banki.png")) {
        std::cerr << "Nie mozna zaladować tekstury baniek: banki.png" << std::endl;
        exit(1);
    }
    bubbleSprite.setTexture(bubbleTexture);
}


void BathroomStrategy::handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (soapSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            isSoapGrabbed = true;
        }
        else {
            roomInteraction.removePoop(mousePos);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        isSoapGrabbed = false;
        soapSprite.setPosition(originalSoapPosition);
        bubbles.clear();
        bubbleTimer.restart();
    }

    if (isSoapGrabbed) {
        soapSprite.setPosition(static_cast<float>(mousePos.x) - soapSprite.getGlobalBounds().width / 2,
            static_cast<float>(mousePos.y) - soapSprite.getGlobalBounds().height / 2);
        checkCollisionWithPuppy(puppySprite, healthBarManager);
        removeStain(window, puppySprite, healthBarManager);
    }
}

void BathroomStrategy::draw(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    window.draw(soapSprite);
    for (auto& bubble : bubbles) {
        window.draw(bubble);
    }
    roomInteraction.drawPoops(window);
    roomInteraction.drawStains(window);
}

//Sprawdza kolizje mydla z psem i dodaje banki
void BathroomStrategy::checkCollisionWithPuppy(sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    if (isSoapGrabbed) {
        auto soapBounds = soapSprite.getGlobalBounds();
        auto puppyBounds = puppySprite.getGlobalBounds();
        float shrinkFactor = 20;
        puppyBounds.left += shrinkFactor;
        puppyBounds.top += shrinkFactor;
        puppyBounds.width -= shrinkFactor * 2;
        puppyBounds.height -= shrinkFactor * 2;

        if (soapBounds.intersects(puppyBounds)) {
            if (bubbleTimer.getElapsedTime().asSeconds() >= 0.1) {
                sf::Vector2f bubblePosition = {
                    std::max(puppyBounds.left, std::min(soapSprite.getPosition().x + soapSprite.getGlobalBounds().width / 2, puppyBounds.left + puppyBounds.width)),
                    std::max(puppyBounds.top, std::min(soapSprite.getPosition().y + soapSprite.getGlobalBounds().height / 2, puppyBounds.top + puppyBounds.height))
                };

                addBubble(bubblePosition);
                bubbleTimer.restart();
            }
        }
    }
}

void BathroomStrategy::addBubble(sf::Vector2f position) {
    sf::Sprite newBubble(bubbleSprite);
    newBubble.setPosition(position);
    bubbles.push_back(newBubble);
}

void BathroomStrategy::removeStain(sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    auto& stains = roomInteraction.getStains();
    auto it = stains.begin();
    while (it != stains.end()) {
        if (soapSprite.getGlobalBounds().intersects(it->getGlobalBounds())) {
            it = stains.erase(it);
            healthBarManager.increaseHygiene(15.0f);
        }
        else {
            ++it;
        }
    }
}

void BathroomStrategy::update(float deltaTime, HealthBarManager& healthBarManager) {}
