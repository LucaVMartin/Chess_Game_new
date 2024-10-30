#pragma once

#include "Board.h"


class Game {
  /* A class that unifies game state and logic */

 private:
  void invalidateAllLegalMoves();
  void nextTurn() { board.isWhiteTurn = !board.isWhiteTurn; }

 public:
  Board board;
  Game();
  ~Game() = default;
  std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
  void move(std::shared_ptr<Piece> piece, int row, int col);
  const void isCheck();
  void resetJustMadeFirstMove();
  bool promotion = false;

  Board::Iterator begin();
  Board::Iterator end();
};
