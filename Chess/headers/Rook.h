#pragma once

#include "Piece.h"

class Rook : public Piece {
public:
	Rook(int row, int col, bool isWhite) : Piece(row, col, isWhite, 5) {};
	using Piece::Piece;
	const std::string getName() const override { return "rook"; }

protected:
	void calculatePossibleMoves(Board& board) override;
};