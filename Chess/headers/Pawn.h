#pragma once

#include "Board.h"
#include "Piece.h"

class Pawn : public Piece {
 public:
  using Piece::Piece;
  const std::string getName() const override { return "pawn"; }
  void calculatePossibleMoves(Board& board) override;
};
