
#include "EventHandler.h"

#include <SFML/Graphics.hpp>

#include "Piece.h"
#include "UI.h"

void EventHandler::handleMouseButtonPressed(sf::Event &e) {
  if (e.mouseButton.button == sf::Mouse::Left) {
    auto [row, col] = ui.coordinatesToIndex({e.mouseButton.x, e.mouseButton.y});
    this->movingPiece = game.getPieceByCoordinates(row, col);

    // Mark the fields of possible Moves green
    if (this->movingPiece && this->movingPiece->isWhite == game.isWhiteTurn) {
      auto PossibleMoves = this->movingPiece->getLegalMoves(game.board);
      ui.CreateRectanglesOfPossibleMoves(PossibleMoves);
    }
  }
}

void EventHandler::handleMouseButtonReleased(sf::Event &e) {
  if (e.mouseButton.button == sf::Mouse::Left) {
    if (this->movingPiece) {
      ui.deleteRectanglesOfPossibleMoves();
      auto piece = this->movingPiece;
      this->movingPiece = nullptr;
      auto [row, col] =
          ui.coordinatesToIndex({e.mouseButton.x, e.mouseButton.y});
      game.move(piece, row, col);
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
    auto mouse = sf::Mouse::getPosition(window);
    if (this->movingPiece) {
      auto windowSize = window.getSize();
      int offset_x = mouse.x % (windowSize.x / 8);
      int offset_y = mouse.y % (windowSize.y / 8);
      ui.movePiece(*(this->movingPiece), mouse.x - offset_x, mouse.y - offset_y);
    }
  }
}