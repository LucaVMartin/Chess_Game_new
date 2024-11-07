#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Piece.h"
#include "Board.h"
#include <iostream>
struct Move {
	Coordinates pieceCoords;
	Coordinates moveCoords;
	bool isPromotion;
	std::string PromotionPiece;
	Move() : pieceCoords(Coordinates(-1, -1)), moveCoords(Coordinates(-1, -1)), isPromotion(false) {};
	Move(Coordinates piece, Coordinates move, bool isPromotion_, std::string PromotionPiece_) : pieceCoords(piece), moveCoords(move), isPromotion(isPromotion_), PromotionPiece(PromotionPiece_) {
		if (isPromotion_) {
			if (PromotionPiece_ != "rook" && PromotionPiece_ != "knight" &&
				PromotionPiece_ != "queen" && PromotionPiece_ != "bishop") {
				std::cout << "This is not a valid promotion Piece!" << std::endl;
			}
		}
	};

	Move& operator=(const Move& other) {
		if (this != &other) { // Check for self-assignment
			this->pieceCoords = other.pieceCoords;
			this->moveCoords = other.moveCoords;
			this->isPromotion = other.isPromotion;
			this->PromotionPiece = other.PromotionPiece;
		}
		return *this;
	}
};
struct evalMove {
	Move move;
	int score;
	evalMove(Move move_ = Move(), int val_ = INT_MIN) : move(move_), score(val_) {};
};

class Engine {

public:
	Engine(bool testing_ = false, bool visualize = false) : testing(testing_) {
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
	//Move currentMove_;
	sf::RenderWindow wind;
	evalMove createTree(Board& board, int depth);
	evalMove alphabeta(Board& board, int depth, int alpha, int beta);
	Move calculateEngineMove(Board& board, int depth);
	//int totalCalcDepth;
private:
	void visualizeBoard(sf::RenderWindow& windowEngine, Board& board);
	int evalPosition(Board& board);
	bool updateCheckStatus(Board& board);
	void countCaptures(Board& board, Coordinates move);
	bool GameEndCheck(Board& board);
	int giveNumPos(int depth);
	const std::vector<Move> moveGenerator(Board& board);
	void createPromotionPiece(Board& board, Move move);
};