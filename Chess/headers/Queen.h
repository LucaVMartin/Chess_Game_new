#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
	Queen(int row, int col, bool isWhite) : Piece(row, col, isWhite, 9) {};
	using Piece::Piece;
	const std::string getName() const override { return "queen"; }

protected:
	void calculatePossibleMoves(Board& board) override;
};