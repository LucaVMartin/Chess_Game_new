#pragma once

#include "Piece.h"

class King : public Piece {
 public:
  using Piece::Piece;
  const std::string getName() const override { return "king"; }

 protected:
  void calculatePossibleMoves(Board& board) override;
  void removeMovesUnderCheck(Board& board);
};