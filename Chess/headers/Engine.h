#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Piece.h"
#include "Board.h"

struct Move {
	Coordinates pieceCoords;
	Coordinates moveCoords;
	Move() : pieceCoords(Coordinates(-1, -1)), moveCoords(Coordinates(-1, -1)) {};
	Move(Coordinates piece, Coordinates move) : pieceCoords(piece), moveCoords(move) {}
};
struct evalMove {
	Move move;
	int val;
};

class Engine {

public:
	Engine(bool testing_ = false,bool visualize = false) : testing(testing_) {
		if (visualize) {
			this->wind.create(sf::VideoMode(800, 800), "Test Window");
			this->wind.setFramerateLimit(30);
		}
	};
	~Engine() {
		if (wind.isOpen()) {
			wind.close();
		}
	}
	static inline int checkctr = 0;
	static inline int checkmatectr = 0;
	static inline int capturectr = 0;
	static inline int positionctr = 0;
	const bool testing;
	sf::RenderWindow wind;
	//Move findBestMove(Board &board, int depth);
	int createTree(Board& board, int depth, Move& bestMove);
	//int totalCalcDepth;
private:
	void visualizeBoard(sf::RenderWindow& windowEngine, Board& board);
	int evalPosition(Board& board);
	bool updateCheckStatus(Board& board);
	void countCaptures(Board& board, Coordinates move);
	bool GameEndCheck(Board& board);
	int giveNumPos(int depth);
};