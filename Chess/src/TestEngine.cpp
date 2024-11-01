#include "Engine.h"
#include <SFML/Graphics.hpp>

#include "EventHandler.h"
#include "Game.h"
#include "UI.h"
#include "Engine.h"

int main(){
    // create window
    sf::RenderWindow window(sf::VideoMode(800, 800), "My Window");
    window.setFramerateLimit(30);
    Game game{};
    UI ui{ &window, game };

    Engine eng_test;

    eng_test.createTree(game.board,10);

    // run Program as long as window is open
    return 0;
}
