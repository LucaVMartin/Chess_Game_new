#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
	Bishop(int row, int col, bool isWhite) : Piece(row, col, isWhite, 3) {};
	using Piece::Piece;
	const std::string getName() const override { return "bishop"; }

protected:
	void calculatePossibleMoves(Board& board) override;
};