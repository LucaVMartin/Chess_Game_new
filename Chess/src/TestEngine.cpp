#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
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
    int ctr = 0;
    auto counter = eng_test.createTree(game.board,5,ctr,window);
    std::cout << "Number of Positions: " << counter;

    // run Program as long as window is open
    return 0;
}
