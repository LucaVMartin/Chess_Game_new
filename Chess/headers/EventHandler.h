#pragma once

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Piece.h"
#include "UI.h"

class EventHandler {
 private:
  Game &game;
  UI &ui;
  sf::RenderWindow &window;
  std::shared_ptr<Piece> movingPiece{nullptr};

 public:
  EventHandler(Game &game, UI &ui, sf::RenderWindow &window)
      : game(game), ui(ui), window(window){};

  void handleEvents();
  void handleMouseButtonPressed(sf::Event &e);
  void handleMouseButtonReleased(sf::Event &e);
};
