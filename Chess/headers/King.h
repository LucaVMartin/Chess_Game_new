#pragma once
#include <limits>
#include "Piece.h"

class King : public Piece {
public:
	using Piece::Piece;
	bool checked = false;
	const std::string getName() const override { return "king"; }
	King(int row, int col, bool isWhite) : Piece(row, col, isWhite, 0) {};
	std::shared_ptr<Piece> clone() const override {
		return std::make_shared<King>(*this); // Copy using the copy constructor
	}
protected:
	void calculatePossibleMoves(Board& board) override;
};