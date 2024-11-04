#pragma once

#include "Board.h"
#include "Piece.h"

class Knight : public Piece {
public:
  using Piece::Piece;
  const std::string getName() const override { return "knight"; }

protected:
  void calculatePossibleMoves(Board &board) override;
};
