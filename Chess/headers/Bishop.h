#pragma once

#include "Piece.h"

class Bishop : public Piece {
 public:
  using Piece::Piece;
  const std::string getName() const override { return "bishop"; }

 protected:
  void calculatePossibleMoves(Board& board) override;
};