#pragma once

#include "Piece.h"

class King : public Piece {
 public:
  using Piece::Piece;
  bool checked = false;
  const std::string getName() const override { return "king"; }

 protected:
  void calculatePossibleMoves(Board& board) override;
};