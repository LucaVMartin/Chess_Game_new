#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
	Bishop(int row, int col, bool isWhite) : Piece(row, col, isWhite, 3) {};
	using Piece::Piece;
	const std::string getName() const override { return "bishop"; }
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<Bishop>(*this); // Copy using the copy constructor
	}
	std::unordered_set<Coordinates> calculatePossibleMoves(Board& board) override;
};