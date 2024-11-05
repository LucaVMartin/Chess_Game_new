#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Piece.h"
#include "Board.h"

struct Move{
		Coordinates pieceCoords;
		Coordinates moveCoords;
	};
struct evalMove {
	Move move;
	int val;
};

class Engine {
	
	public:
		static inline int checkctr = 0;
		static inline int checkmatectr = 0;
		static inline int capturectr = 0;
		static inline int positionctr = 0;
		//Move findBestMove(Board &board, int depth);
		int createTree(Board& board, int depth, sf::RenderWindow& renderwind,Move& bestMove);
	    //int totalCalcDepth;
	private:
		void visualizeBoard(sf::RenderWindow& windowEngine, Board& board);
		int evalPosition(Board& board);
		bool updateCheckStatus(Board& board);
		void countCaptures(Board& board,Coordinates move);
		bool checkmateCounter(Board& board);
};