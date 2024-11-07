#pragma once

#include "Board.h"
#include "Piece.h"

class Pawn : public Piece {
public:
	Pawn(int row, int col, bool isWhite) : Piece(row, col, isWhite, 1) {};
	using Piece::Piece;
	const std::string getName() const override { return "pawn"; }
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<Pawn>(*this); // Copy using the copy constructor
	}
	std::unordered_set<Coordinates> calculatePossibleMoves(Board& board) override;

};
