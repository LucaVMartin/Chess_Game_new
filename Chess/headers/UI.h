#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

#include "Game.h"
#include "Piece.h"

class UI {
private:
  /* data */
  sf::RenderWindow *window;
  sf::Sprite board{};
  std::unordered_map<std::string, sf::Texture> textureByName;
  std::unordered_map<unsigned, sf::Sprite> spriteByPieceId;
  std::vector<sf::RectangleShape> RectanglesofPossibleMoves;
  Piece *movingPiece; // draw on top
  sf::Sprite createPieceSprite(const Piece &piece);
  void createBoard();
  void _loadAllTextures();
  sf::Texture *getTexture(const Piece &piece);
  sf::Sprite *getSprite(const Piece &piece);

  sf::Vector2f getWindowSize() const {
    sf::Vector2f windowSize(window->getSize());
    return windowSize;
  }

public:
  UI(sf::RenderWindow *window, Game &game);
  ~UI() = default;

  void drawBoard();
  void resizeBoard();
  void setUItoGame(Game &game);
  void movePiece(Piece &piece, int x, int y);
  void
  CreateRectanglesOfPossibleMoves(const std::set<Coordinates> &PossibleMoves);
  void deleteRectanglesOfPossibleMoves();
  sf::Vector2f indexToCoordinates(int row, int col);
  std::pair<int, int> coordinatesToIndex(sf::Vector2i coordinates);
};
