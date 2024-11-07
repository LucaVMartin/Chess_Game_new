#pragma once

#include "Piece.h"

class Rook : public Piece {
public:
	Rook(int row, int col, bool isWhite) : Piece(row, col, isWhite, 5) {};
	using Piece::Piece;
	const std::string getName() const override { return "rook"; }
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<Rook>(*this); // Copy using the copy constructor
	}
protected:
	std::unordered_set<Coordinates> calculatePossibleMoves(Board& board) override;
};