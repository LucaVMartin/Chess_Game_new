#pragma once

#include "Piece.h"

class Rook : public Piece {
 public:
  using Piece::Piece;
  const std::string getName() const override { return "rook"; }

 protected:
  void calculatePossibleMoves(Board& board) override;
};