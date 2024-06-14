#include"Game.h"

/*
KITCHENSTRATEGY:
* FILESYSTEM - Ładowanie tekstur jedzenia z katalogu resources

PUPPYNAMINGMENU.H:
* REGEX - Imie psa musi rozpoczynać się z dużej litery i zawierać TYLKO literki, definiuje wzorzec imienia 
 
 HEALTHBARMANAGER:
* RANGES - filtruje czy, któryś z pasków ma mniej niż 25% zdrowia, jak tak to zmienia sprit'a psa

 PLAYROOMSTRATEGY:
* THREADS - ThrowThread symuluje ruch piłki w tle, aktualizując jej pozycje oraz obsługując odbicia od krawędzi okna, pozwala na kontynuacje
działania głównej pętli bez blokowania interfejsu 

* MUTEX - tylko jeden wątek na raz ma dostęp do zmiennych związanych z piłką, zapobiega problemom związanym z konkurencją 
wątków i zapewnia integralność danych u nas MUTEX pozwala nam na rzucanie piłką oraz łapanie jej nawet w locie oraz ponowny rzut

* CONDITION VARIABLE - zmienna warunkowa 'cv' używana w metodzie throwBall do kontrolowania cykli wątku, wątek czeka na zmienną warunkową przez 
 10 milisekund lub do momentu gdy running stanie się false. Umożliwia płynne zatrzymanie i wznowienie piłki w odpowiedzi na interakcje użytkownika

 * DZIEDZICZENIE - RoomStrategy (bazowa) - BedroomStrategy, BathroomStrategy, KitchenStrategy, PlayRoomStrategy (pochodne)
PuppyBuilder(bazowa) - BordiBuilder, MaltiBuilder, GoldiBuilder (pochodne), Menu(bazowa) - PuppyNamingMenu (pochodna)

* POLIMORFIZM - Metody wirtualne handleInteraction, draw, update są implementowane w klasach pochodnych i wywoływane przez wskaźnik do RoomStrategy
RoomInteraction używa wskaźnika currentStrategy do przechowywania obiektu RoomStrategy i wywoływania metod w zależności od bieżącej strategii
Metody wirtualne buildHappyTexture, buildSadTexture i buildSleepTexture są implementowane w klasach pochodnych BordiBuilder itd
Menu: Metodaw draw jest przesłonięta w klasie PuppyNamingMenu (override)

* ABSTRAKCJA - Klasy RoomStrategy i PuppyBuilder są abstrakcyjne (zawierają metody czysto wirtualne)

* ZARZĄDZANIE PAMIĘCIĄ - użycie inteligentych wskaźników (std::unique_ptr, std::shared_ptr) w KitchenStrategy oraz w RoomInteraction

* WZORCE PROJEKTOWE:
Budowniczy - umożliwia tworzenie różnych reprezentacji tego samego obiektu 
Strategia - Pozwala definiować rodziny algorytmów, umieszczać je w osobnych klasach i sprawiać, że są wymienne. Umożliwia łatwą wymianę oraz dodawanie
nowych strategii

*/

using namespace std;

int main() {
    runGame();
    return 0;
}
