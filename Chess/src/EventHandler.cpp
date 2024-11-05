
#include "EventHandler.h"

#include <SFML/Graphics.hpp>

#include "Piece.h"
#include "UI.h"

void EventHandler::handleMouseButtonPressed(sf::Event& e) {

	if (e.mouseButton.button == sf::Mouse::Left) {
		if (game.board.promotion) {
			return;
		}

		auto [row, col] = ui.coordinatesToIndex({ e.mouseButton.x, e.mouseButton.y });
		if (row < 8 && row >= 0 && col >= 0 && col < 8)
			this->movingPiece = game.getPieceByCoordinates(row, col);
		else
			this->movingPiece = nullptr;

		// Mark the fields of possible Moves green
		if (this->movingPiece && this->movingPiece->isWhite == game.board.isWhiteTurn) {
			auto PossibleMoves = this->movingPiece->posMoves;
			ui.CreateRectanglesOfPossibleMoves(PossibleMoves);
		}
	}
}

void EventHandler::handleMouseButtonReleased(sf::Event& e) {
	if (e.mouseButton.button == sf::Mouse::Left) {

		if (this->movingPiece) {
			ui.deleteRectanglesOfPossibleMoves();
			auto piece = this->movingPiece;
			this->movingPiece = nullptr;
			auto [row, col] =
				ui.coordinatesToIndex({ e.mouseButton.x, e.mouseButton.y });

			//moves piece, sets next turn, checks for check ...
			game.moveProcedure(piece, row, col);

			if (game.board.promotion) {
				ui.promotionUI(piece->isWhite);
			}
			ui.setUItoGame(game);
			return;
		}
		if (game.board.promotion) {
			std::string piece = ui.promotionSelector({ e.mouseButton.x, e.mouseButton.y });
			if (piece == "nopiece") return;
			game.board.createPromotionPiece(piece);
			game.isCheck();
			game.invalidateAllLegalMoves(); //deletes all possible moves of the pieces
			game.calculateAllLegalMoves(); //calculates legal moves for all pieces
			ui.deletePromotionUI();
			game.board.promotion = false;
			ui.setUItoGame(game);
		}
	}
}

void EventHandler::handleEvents() {
	sf::Event event{};

	while (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonPressed:
			handleMouseButtonPressed(event);
			break;
		case sf::Event::MouseButtonReleased:
			handleMouseButtonReleased(event);
			break;
		default:
			break;
		}
		//moves grabbed pieces
		auto mouse = sf::Mouse::getPosition(window);
		if (this->movingPiece) {
			auto windowSize = window.getSize();
			int offset_x = mouse.x % (windowSize.x / 8);
			int offset_y = mouse.y % (windowSize.y / 8);
			ui.movePiece(*(this->movingPiece), mouse.x - offset_x, mouse.y - offset_y);
		}
	}
}