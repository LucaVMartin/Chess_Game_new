#pragma once

#include "Piece.h"

class Queen : public Piece {
 public:
  using Piece::Piece;
  const std::string getName() const override { return "queen"; }

 protected:
  void calculatePossibleMoves(Board& board) override;
};