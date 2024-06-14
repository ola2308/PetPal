#include "RoomInteraction.h"

RoomInteraction::RoomInteraction(sf::RenderWindow& win) : window(win), currentStrategy(nullptr) {
    if (!poopTexture.loadFromFile("reszta/poop.png")) {
        std::cout << "Nie mozna zaladowac tekstury poop: poop.png" << std::endl;
        exit(1);
    }
}

//ustawianie wskaźnika na healthbarmanager
void RoomInteraction::setHealthBarManager(HealthBarManager* manager) {
    healthBarManager = manager;
}

void RoomInteraction::setStrategy(RoomStrategy* strategy) {
    currentStrategy.reset(strategy);
}

void RoomInteraction::handleInteraction(int roomIndex, sf::Event event, sf::Sprite& puppySprite) {
    if (currentStrategy) {
        currentStrategy->handleInteraction(event, window, puppySprite, *healthBarManager);
    }
}

void RoomInteraction::draw(sf::Sprite& puppySprite, int roomIndex, sf::RenderWindow& window) {
    if (currentStrategy) {
        currentStrategy->draw(window, puppySprite);
    }
}

std::vector<sf::CircleShape>& RoomInteraction::getStains() {
    return stains;
}

std::vector<sf::Sprite>& RoomInteraction::getPoops() {
    return poops;
}

void RoomInteraction::addPoop(sf::Sprite& puppySprite) {
    sf::Sprite poop(poopTexture);
    float xOffset = 80 - rand() % 100;  // losowe przesunięcie wokół psa
    float yOffset = 250 - rand() % 100;
    poop.setPosition(puppySprite.getPosition().x - xOffset, puppySprite.getPosition().y + yOffset);
    poops.push_back(poop);
    std::cout << "Dodano kupę na pozycji: (" << poop.getPosition().x << ", " << poop.getPosition().y << ")" << std::endl;
}

void RoomInteraction::addStain(sf::Sprite& puppySprite, int transparency) {
    sf::CircleShape newStain(18);
    newStain.setFillColor(sf::Color(139, 69, 19, transparency));  // Brązowy kolor z zadaną przezroczystością

    // Losowa pozycja na sprite'cie psa
    float xOffset = static_cast<float>(rand() % static_cast<int>(puppySprite.getGlobalBounds().width - newStain.getRadius() * 2 - 80));
    float yOffset = static_cast<float>(rand() % static_cast<int>(puppySprite.getGlobalBounds().height - newStain.getRadius() * 2 - 80));
    newStain.setPosition(puppySprite.getPosition().x + xOffset + newStain.getRadius(), puppySprite.getPosition().y + yOffset + newStain.getRadius());
    stains.push_back(newStain);
}

void RoomInteraction::drawBubbles(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    for (auto& bubble : bubbles) {
        window.draw(bubble);
    }
}

void RoomInteraction::drawStains(sf::RenderWindow& window) {
    for (auto& stain : stains) {
        window.draw(stain);
    }
}

int RoomInteraction::getCurrentRoomIndex() const {
    return currentRoomIndex;
}

void RoomInteraction::drawPoops(sf::RenderWindow& window) {
    for (auto& poop : poops) {
        window.draw(poop);
    }
}

void RoomInteraction::setCurrentRoomIndex(int index) {
    currentRoomIndex = index;
    std::cout << "Aktualny pokoj ustawiony na: " << currentRoomIndex << std::endl;
}

void RoomInteraction::removePoop(sf::Vector2i mousePosition) {
    auto it = poops.begin();
    while (it != poops.end()) {
        if (it->getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            it = poops.erase(it);
            if (healthBarManager) {
                healthBarManager->increaseHygiene(10.0f);
            }
        } else {
            ++it;
        }
    }
}

void RoomInteraction::openFridge() {
    setStrategy(new KitchenStrategy(window));
    std::cout << "Lodowka otwarta" << std::endl;
}

RoomStrategy* RoomInteraction::getCurrentStrategy() const {
    return currentStrategy.get();
}

void RoomInteraction::saveState(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out);
    if (!outFile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku do zapisu: " << filename << std::endl;
        return;
    }

    outFile << poops.size() << std::endl;
    for (const auto& poop : poops) {
        outFile << poop.getPosition().x << " " << poop.getPosition().y << std::endl;
    }

    outFile << stains.size() << std::endl;
    for (const auto& stain : stains) {
        outFile << stain.getPosition().x << " " << stain.getPosition().y << std::endl;
    }

    outFile.close();
}

void RoomInteraction::loadState(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::in);
    if (!inFile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku do odczytu: " << filename << std::endl;
        return;
    }

    size_t poopCount;
    inFile >> poopCount;
    poops.clear(); //możemy kontynuować gre bo jest miejsce na nowe kupki
    for (size_t i = 0; i < poopCount; ++i) {
        float x, y;
        inFile >> x >> y;
        sf::Sprite poop(poopTexture);
        poop.setPosition(x, y);
        poops.push_back(poop);
    }

    size_t stainCount;
    inFile >> stainCount;
    stains.clear();
    for (size_t i = 0; i < stainCount; ++i) {
        float x, y;
        inFile >> x >> y;
        sf::CircleShape stain(20);
        stain.setFillColor(sf::Color(139, 69, 19, 120));
        stain.setPosition(x, y);
        stains.push_back(stain);
    }

    inFile.close();
}
