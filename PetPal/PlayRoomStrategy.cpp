#include "PlayRoomStrategy.h"


//£adowanie tekstury pi³ki i ustawianie jej pozycji
PlayRoomStrategy::PlayRoomStrategy(sf::RenderWindow& window)
    : isDragging(false), running(false), bounceCount(0) {
    if (!ballTexture.loadFromFile("reszta/ball1.png")) {
        std::cerr << "Nie mozna zaladowac tekstury: ball1.png" << std::endl;
    }
    ballSprite.setTexture(ballTexture);
    initialPosition = sf::Vector2f(window.getSize().x - ballTexture.getSize().x, window.getSize().y - ballTexture.getSize().y);
    ballSprite.setPosition(initialPosition);
}


//Zamykanie w¹tku throwThread w bezpieczny sposób, ustawienie running na false i powiadomienie oczekuj¹cych w¹tków o zmianie stanu, unikamy m.in.wycieków pamiêci
PlayRoomStrategy::~PlayRoomStrategy() {
    {
        std::lock_guard<std::mutex> lock(ballMutex);
        running = false;
    }
    cv.notify_all();
    if (throwThread.joinable()) {
        throwThread.join();
    }
}


//Obs³uguje interakcje pi³k¹, po jej upuszczeniu uruchamia w¹tek throwThread, aktualizuje jej pozycje, nadpisana funkcja klasy bazowej
void PlayRoomStrategy::handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && ballSprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
        {
            std::lock_guard<std::mutex> lock(ballMutex);
            isDragging = true;
            dragOffset = window.mapPixelToCoords(mousePos) - ballSprite.getPosition();
            running = false;
            cv.notify_all();
        }
        if (throwThread.joinable()) {
            throwThread.join();
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && isDragging) {
        {
            std::lock_guard<std::mutex> lock(ballMutex);
            isDragging = false;
            initialVelocity = (window.mapPixelToCoords(mousePos) - ballSprite.getPosition()) * 2.0f;
            healthBarManager.increaseHappiness(10.0f);
            running = true;
            bounceCount = 0;
        }
        throwThread = std::thread(&PlayRoomStrategy::throwBall, this, std::ref(window), std::ref(healthBarManager));
    }
    else if (event.type == sf::Event::MouseMoved && isDragging) {
        std::lock_guard<std::mutex> lock(ballMutex);
        ballSprite.setPosition(window.mapPixelToCoords(mousePos) - dragOffset);
    }
}

//nadpisane funkcje klasy bazowej
void PlayRoomStrategy::draw(sf::RenderWindow& window, sf::Sprite& puppySprite) {
    window.draw(ballSprite);
}

void PlayRoomStrategy::update(float deltaTime, HealthBarManager& healthBarManager) {}

//symuluje ruch pi³ki, aktualizuje jej pozycje, obs³uguje odbicia, zwiêksza poziom szczêœcia przy odbiciach, zatrzymuje symulacje po 3 odbiciach
void PlayRoomStrategy::throwBall(sf::RenderWindow& window, HealthBarManager& healthBarManager) {
    sf::Vector2f velocity = initialVelocity;
    sf::Clock clock;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(ballMutex);//zapewnia bezpieczne blokowanie i oblokowywanie mutexa, zapobiegajac b³êdom w zarz¹dzaniu mutexa
            cv.wait_for(lock, std::chrono::milliseconds(10), [this] { return !running; });

            if (!running) {
                break;
            }

            sf::Time elapsed = clock.restart();
            ballSprite.move(velocity * elapsed.asSeconds());

            // Odbijanie od œcian
            if (ballSprite.getPosition().x <= 0 || ballSprite.getPosition().x + ballSprite.getGlobalBounds().width >= window.getSize().x) {
                velocity.x = -velocity.x;
                bounceCount++;
            }
            if (ballSprite.getPosition().y <= 0 || ballSprite.getPosition().y + ballSprite.getGlobalBounds().height >= window.getSize().y) {
                velocity.y = -velocity.y;
                bounceCount++;
            }

            healthBarManager.increaseHappiness(0.1f);

            if (bounceCount >= 3) {
                ballSprite.setPosition(initialPosition);
                running = false;
                break;
            }
        }
    }
}
