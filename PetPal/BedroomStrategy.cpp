#include "BedroomStrategy.h"

//Klasa BedroomStrategy dziedziczy po klasie abstrakcyjnej RoomStrategy

//Konstruktor
BedroomStrategy::BedroomStrategy(sf::RenderWindow& win, Puppy* p)
    : window(win), puppy(p), isNight(false), sleepAccumulator(0.0f) {
    if (!nightTexture.loadFromFile("reszta/sypialnia_noc.png")) {
        std::cerr << "Nie udalo sie zaladowac: sypialnia_noc.png" << std::endl;
        exit(1);
    }
    nightSprite.setTexture(nightTexture);
    nightSprite.setPosition(0, 0);
}

//obsługuje kliknięcie myszy na lampie, przełącza tryb dzień/noc 
void BedroomStrategy::handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect lampBounds(610.f, 160.f, 130.f, 110.f);

        if (lampBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            isNight = !isNight;
            if (puppy) {
                puppy->isSleeping = isNight;
                puppy->updateTexture(!healthBarManager.isHealthCritical());
                drawPuppy(window, puppySprite);
            }

            std::cout << (isNight ? "noc" : "dzien") << std::endl;
            sleepAccumulator = 0.0f;
        }
    }
}

//Jeśli jest noc to zwiększa pasek zdrowia o 10% na sekunde, nadpisana funkcja z klasy bazowej, deltaTime - czas, który upłynął od ostatniego wywołania funkcji
void BedroomStrategy::update(float deltaTime, HealthBarManager& healthBarManager) {
    if (isNight) {
        sleepAccumulator += deltaTime;
        if (sleepAccumulator >= 1.0f) {
            healthBarManager.increaseSleep(10.0f);
            sleepAccumulator = 0.0f;
        }
    }
}

//Rysuje sprite'a psa nocnego gdy jest noc, nadpisana z klady bazowej RoomStrategy
void BedroomStrategy::draw(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    if (isNight) {
        window.draw(nightSprite);
    }
    drawPuppy(window, puppySprite);
}

//rysuje psa
void BedroomStrategy::drawPuppy(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    if (puppy) {
        window.draw(puppy->sprite);
    }
}
