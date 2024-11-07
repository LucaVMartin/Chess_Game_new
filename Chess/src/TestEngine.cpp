#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EventHandler.h"
#include "Game.h"
#include "UI.h"
#include "Engine.h"

int main() {
	Game game(true); //with engine
	Engine eng_test;
	sf::RenderWindow window;
	int depth;
	Move best;
	window.create(sf::VideoMode(800, 800), "ChessGame");
	window.setFramerateLimit(30);
	UI ui{ &window, game };
	EventHandler e{ game, ui, window };

	depth = 5; // Set depth for testing
	bool madeMove = true;
	while (window.isOpen()) {
		if (game.playEngine && !game.board.isWhiteTurn) {
			auto move = game.performEngineMove(depth);
			ui.CreateRectanglesOfPerformedMove(move.pieceCoords, move.moveCoords);
			ui.setUItoGame(game);
		}
		e.handleEvents();
		window.clear();
		ui.drawBoard();
		window.display();
	}
	return 0;
}
