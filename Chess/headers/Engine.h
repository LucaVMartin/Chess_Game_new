#pragma once
#include <SFML/Graphics.hpp>
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
		int createTree(Board& board, int depth, int& counter, sf::RenderWindow& renderwind);
		int createNoCopyTree(Board& board, int depth, int& counter, sf::RenderWindow& wind);
	    //int totalCalcDepth;
	private:
		void visualizeBoard(sf::RenderWindow& windowEngine, Board& board);
		int evalPosition(Board& board);

};