#pragma once

#include "Board.h"
#include "Piece.h"

class Knight : public Piece {
public:
	Knight(int row, int col, bool isWhite) : Piece(row, col, isWhite, 3) {};
	using Piece::Piece;
	const std::string getName() const override { return "knight"; }
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<Knight>(*this); // Copy using the copy constructor
	}
protected:
	void calculatePossibleMoves(Board& board) override;
};
