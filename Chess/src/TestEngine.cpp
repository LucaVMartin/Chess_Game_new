#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EventHandler.h"
#include "Game.h"
#include "UI.h"
#include "Engine.h"

int main() {
	// create window
	sf::RenderWindow window(sf::VideoMode(800, 800), "My Window");
	window.setFramerateLimit(30);
	Game game{};
	//UI ui{ &window, game };

	Engine eng_test;
	int ctr = 0;
	int depth =5;
	int numpos = 0;

	switch (depth) {
	case 1: numpos = 20; break;
	case 2: numpos = 400; break;
	case 3: numpos = 8902; break;
	case 4: numpos = 197281; break;
	case 5: numpos = 4865609; break;
	case 6: numpos = 119060324; break;
	default: numpos = 0; break; // Handle invalid depths
	}

	auto counter = eng_test.createTree(game.board, depth, ctr, window);
	std::cout << "enpas: " << game.board.enpassantctr;

	std::cout << "Number of Positions: " << counter<<std::endl;
	std::cout << "diff = " << numpos - counter;
	// run Program as long as window is open
	return 0;
}
