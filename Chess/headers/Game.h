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
  std::string checkGameEnd(std::shared_ptr<Piece> attackPiece);
  std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
  bool move(std::shared_ptr<Piece> piece, int row, int col);
  const std::shared_ptr<Piece> isCheck();

  Board::Iterator begin();
  Board::Iterator end();
};
