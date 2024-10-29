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
	bool isWhiteTurn = true;
	std::shared_ptr<Piece> whiteKing;
	std::shared_ptr<Piece> blackKing;
	Board();
	std::array<std::shared_ptr<Piece>, 8>& operator[](int row);
	size_t size();

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