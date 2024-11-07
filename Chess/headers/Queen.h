#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
	Queen(int row, int col, bool isWhite) : Piece(row, col, isWhite, 9) {};
	using Piece::Piece;
	const std::string getName() const override { return "queen"; }
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<Queen>(*this); // Copy using the copy constructor
	}
protected:
	std::unordered_set<Coordinates> calculatePossibleMoves(Board& board) override;
};