#include "KitchenStrategy.h"

namespace fs = std::filesystem;

KitchenStrategy::KitchenStrategy(sf::RenderWindow& win) : window(win) {
    // Inicjalizacja prostokąta lodówki
    fridgeRect.setSize(sf::Vector2f(200, 490));
    fridgeRect.setFillColor(sf::Color::Transparent);
    fridgeRect.setOutlineColor(sf::Color::Transparent);
    fridgeRect.setOutlineThickness(2);
    fridgeRect.setPosition(550, 200);

    // Ładowanie tekstury lodówki
    if (!fridgeTexture.loadFromFile("resources/lodowka.png")) {
        std::cerr << "Nie mozna zaladowac tekstury lodowki: resources/lodowka.png" << std::endl;
        exit(1);
    }
    fridgeSprite.setTexture(fridgeTexture);

    // Inicjalizacja sprite'ów jedzenia
    initializeFoodSprites();
}

void KitchenStrategy::initializeFoodSprites() {
    // Ładowanie tekstur jedzenia z katalogu "resources/foods"
    std::string foodDirectory = "resources/foods";
    if (fs::exists(foodDirectory) && fs::is_directory(foodDirectory)) {
        for (const auto& entry : fs::directory_iterator(foodDirectory)) {
            if (entry.is_regular_file()) {
                auto texture = std::make_shared<sf::Texture>();//tworzenie inteligentnych wskaźników
                std::cout << "Proba zaladowania tekstury: " << entry.path().string() << std::endl;
                if (texture->loadFromFile(entry.path().string())) {
                    std::cout << "Zaladowano teksture: " << entry.path().string() << std::endl;
                    foodTextures.push_back(texture);
                    sf::Sprite sprite(*texture);
                    foodSprites.push_back(sprite);
                }
                else {
                    std::cerr << "Nie mozna zaladowac tekstury: " << entry.path().string() << std::endl;
                }
            }
        }
    }
    else {
        std::cerr << "Katalog " << foodDirectory << " nie istnieje lub nie jest katalogiem." << std::endl;
    }
}

void KitchenStrategy::handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (!isFridgeOpen && fridgeRect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            openFridge();
        }
        else if (isFridgeOpen) {
            handleFoodInteraction(event, window, puppySprite, healthBarManager);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        handleFoodRelease(event, window, puppySprite, healthBarManager);
    }
    else if (event.type == sf::Event::MouseMoved && isFoodHeld) {
        heldFood.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
}

void KitchenStrategy::draw(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    window.draw(fridgeRect);
    if (isFridgeOpen) {
        window.draw(fridgeSprite);
        drawFood(window);
    }
    if (isFoodHeld) {
        window.draw(heldFood);
    }
}

void KitchenStrategy::openFridge() {
    isFridgeOpen = true;
    fridgeSprite.setPosition(window.getSize().x / 2 - fridgeSprite.getGlobalBounds().width / 2, window.getSize().y / 2 - fridgeSprite.getGlobalBounds().height / 2);
}

void KitchenStrategy::closeFridge() {
    isFridgeOpen = false;
}

void KitchenStrategy::drawFood(sf::RenderWindow& window) {
    float yOffset1 = 230;
    float yOffset2 = 490;
    float xOffsetStart = 150;
    float xOffsetStep = 180;

    for (size_t i = 0; i < foodSprites.size(); ++i) {
        if (i < 3) {
            foodSprites[i].setPosition(xOffsetStart + i * xOffsetStep, yOffset1);
        }
        else {
            foodSprites[i].setPosition(xOffsetStart + (i - 3) * xOffsetStep, yOffset2);
        }
        window.draw(foodSprites[i]);
    }
}

void KitchenStrategy::handleFoodInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (size_t i = 0; i < foodSprites.size(); ++i) {
            if (foodSprites[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                isFoodHeld = true;
                heldFood = foodSprites[i];
                heldFood.setScale(0.75f, 0.75f);
                break;
            }
        }
        closeFridge();
    }
}

void KitchenStrategy::handleFoodRelease(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    if (isFoodHeld) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        isFoodHeld = false;

        if (puppySprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            float yOffset1 = 230;
            float yOffset2 = 470;

            if (heldFood.getPosition().y >= yOffset1 && heldFood.getPosition().y < yOffset1 + 150) {
                healthBarManager.increaseFood(15.0f);
            }
            else if (heldFood.getPosition().y >= yOffset2 && heldFood.getPosition().y < yOffset2 + 150) {
                healthBarManager.increaseFood(25.0f);
            }
        }
        heldFood.setScale(1.0f, 1.0f);
        heldFood.setPosition(-100.0f, -100.0f);
    }
}

void KitchenStrategy::update(float deltaTime, HealthBarManager& healthBarManager) {}
