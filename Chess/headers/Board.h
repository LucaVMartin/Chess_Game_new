#pragma once

#include <array>
#include <iterator>
#include <memory>

#include "Piece.h"
#include "King.h"

using BoardType = std::array<std::array<std::shared_ptr<Piece>, 8>, 8>;
class Board {
	//Includes pointers of to all pieces and iterators to loop over them. Also implements convenient way to access the different fields of the board with board[row][col]
private:
	BoardType board;

public:
	Coordinates Promotion;
	bool promotion = false;
	bool isWhiteTurn = true;
	std::shared_ptr<Piece> whiteKing;
	std::shared_ptr<Piece> blackKing;
	std::shared_ptr<Piece> firstMovedPiece;
	static inline int enpassantctr = 0;

	Board();
	Board(const Board& other);
	void createPromotionPiece(std::string pieceName);
	std::array<std::shared_ptr<Piece>, 8>& operator[](int row);
	size_t size();
	bool move(std::shared_ptr<Piece> piece, int row, int col);
	class Iterator {
		/* Iterates through the pieces contained in the board */
	public:
		BoardType& board;
		size_t i_row = 0;
		size_t i_col = 0;

		Iterator(BoardType& board);
		Iterator(BoardType& board, bool isEnd);

		Iterator& operator++();
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
		std::shared_ptr<Piece> operator*();
	};
	Iterator begin();
	Iterator end();

};