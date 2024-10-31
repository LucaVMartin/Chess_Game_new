#pragma once

#include "Board.h"


class Game {
  /* A class that unifies game state and logic */

 private:
  
  void nextTurn() { board.isWhiteTurn = !board.isWhiteTurn; }

 public:
  Board board;
  Game();
  ~Game() = default;

  void moveProcedure(std::shared_ptr<Piece> piece, int row, int col);
  void invalidateAllLegalMoves();
  void calculateAllLegalMoves();
  bool checkGameEnd();
  std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
  bool move(std::shared_ptr<Piece> piece, int row, int col);
  const void isCheck();
  void resetJustMadeFirstMove();
  bool promotion = false;

  Board::Iterator begin();
  Board::Iterator end();
};
