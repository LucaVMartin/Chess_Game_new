#pragma once
#include <memory>
#include "Piece.h"
#include "Board.h"

struct Move{
		std::shared_ptr<Piece> piece;
		Coordinates moveCoords;
	};
struct evalMove {
	Move move;
	int val;
};

class Engine {
	
	public:
		//Move findBestMove(Board &board, int depth);
		int createTree(Board& board, int depth);
	    //int totalCalcDepth;
	private:
		const void visualizeBoard(Board& board);
		int evalPosition(Board& board);

};