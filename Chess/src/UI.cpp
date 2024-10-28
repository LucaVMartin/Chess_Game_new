#include "UI.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "Pawn.h"
#include "Piece.h"
#include "unordered_set"

// helper functions
void _import_textures(sf::Texture &Figure_t, const std::string &texture_file) {
  std::string directory = "Images/" + texture_file;
  //std::string directory = "C:\\Users\\User\\OneDrive\\Dokumente\\CodingProjects\\SFML_Project\\Chess\\Images\\" + texture_file;
  if (!Figure_t.loadFromFile(directory)) {
    std::cout << "Failed Loading texture of " << texture_file << std::endl;
  }
}

void _resize_figures(sf::Sprite &sprite, const sf::Vector2f &windowsize) {
  sprite.setScale((windowsize.x / 8) / sprite.getLocalBounds().width,
                  (windowsize.y / 8) / sprite.getLocalBounds().height);
}

// UI class implementations
UI::UI(sf::RenderWindow *window, Game &game) : window(window) {
  _loadAllTextures();
  createBoard();
  setUItoGame(game);
}
void UI::_loadAllTextures() {
  // requires all textures to be in /Images and have an appropiate naming scheme
  auto pieceNames = {"pawn",  "rook",   "bishop", "knight",
                     "queen", "king"};
  auto colors = {"white", "black"};
  std::string suffix = ".png";

  for (std ::string piece : pieceNames) {
    for (std ::string color : colors) {
      std::string prefix = piece + "_" + color;
      sf::Texture texture;
      _import_textures(texture, prefix + suffix);
      textureByName.insert({prefix, texture});
    }
  }
  sf::Texture boardTexture;
  _import_textures(boardTexture, "Board.png");
  textureByName.insert({"board", boardTexture});
}
sf::Vector2f UI::indexToCoordinates(int row, int col) {
  size_t boardLength = 8;
  auto windowSize = getWindowSize();
  float gridsize_x = windowSize.x / boardLength;
  float gridsize_y = windowSize.y / boardLength;
  sf::Vector2f coordinates{};
  coordinates.x = gridsize_x * col;
  coordinates.y = gridsize_y * (boardLength - row - 1);
  // y-coordinate is inverted (highest value is on the bottom)
  return coordinates;
}

std::pair<int, int> UI::coordinatesToIndex(sf::Vector2i coordinates) {
  auto windowsize = getWindowSize();
  int col = static_cast<int>(coordinates.x / windowsize.x * 8);
  int row = static_cast<int>((windowsize.y - coordinates.y) / windowsize.y * 8);
  // y-coordinate is inverted (highest value is on the bottom)
  return {row, col};
}

void UI::createBoard() {
  board.setTexture(textureByName["board"]);
  resizeBoard();
}

void UI::resizeBoard() {
  auto window_size = getWindowSize();
  board.setScale(window_size.x / board.getLocalBounds().width,
                 window_size.y / board.getLocalBounds().height);
}

void UI::drawBoard() {
  // Draw Board
  window->draw(board);
  // Draw Figures
  for (auto& piece : spriteByPieceId) {
    window->draw(piece.second);
  }
 
 //Currently the whole board is redrawn every loop so this does nothing
 if (movingPiece) {
    auto sprite = getSprite(*movingPiece);
    window->draw(*sprite);
  }

  // Draw Possible Moves
  for (auto& Rectangle : RectanglesofPossibleMoves) {
    window->draw(Rectangle);
  }
}

sf::Texture *UI::getTexture(const Piece &piece) {
  std::string type = piece.getName();
  std::string color = piece.isWhite ? "white" : "black";
  auto textureKey = type + "_" + color;
  return &textureByName[textureKey];
}

sf::Sprite *UI::getSprite(const Piece &piece) {
  return &spriteByPieceId[piece.id];
}

void UI::movePiece(Piece &piece, int x, int y) {
  /* moves piece to the coordinates
  x and y need to bee SFML compatible coordinates of the windows*/
  movingPiece = &piece;
  auto sprite = getSprite(piece);
  sprite->setPosition({ static_cast<float>(x), static_cast<float>(y)});
}

sf::Sprite UI::createPieceSprite(const Piece &piece) {
  sf::Sprite pieceSprite;
  pieceSprite.setTexture(*getTexture(piece));

  _resize_figures(pieceSprite, getWindowSize());

  return pieceSprite;
}

void UI::setUItoGame(Game &game) {
  spriteByPieceId.clear();
  for (auto piece : game) {
    auto sprite = createPieceSprite(*piece);
    spriteByPieceId.insert_or_assign((*piece).id, sprite);
    auto [row, col] = piece->getCurrentField();
    auto coordinates = indexToCoordinates(row, col);
    movePiece(*piece, static_cast<int>(coordinates.x), static_cast<int>(coordinates.y));
  }
}

void UI::CreateRectanglesOfPossibleMoves(
    const std::unordered_set<Coordinates> &PossibleMoves) {
  for (auto &Move : PossibleMoves) {
    auto RectSize1 = (this->getWindowSize().x / 8);
    auto RectSize2 = (this->getWindowSize().y / 8);
    sf::RectangleShape Rectangle(sf::Vector2f(RectSize1, RectSize2));
    Rectangle.setFillColor(sf::Color(0, 255, 0, 50));
    auto coordinates = indexToCoordinates(Move.row, Move.col);
    Rectangle.setPosition(coordinates);
    RectanglesofPossibleMoves.push_back(Rectangle);
  }
}

void UI::deleteRectanglesOfPossibleMoves() {
  RectanglesofPossibleMoves.clear();
}
