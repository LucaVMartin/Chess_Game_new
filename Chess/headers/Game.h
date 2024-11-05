#pragma once

#include "Board.h"
#include "Engine.h"

class Game {
	/* A class that unifies game state and logic */

private:

	void nextTurn() { board.isWhiteTurn = !board.isWhiteTurn; }

public:
	const bool playEngine;
	Board board;
	Engine engine;
	Game(bool playEngine_ = false) : playEngine(playEngine_), board(), engine(false, true) {
		this->calculateAllLegalMoves();
	};
	~Game() = default;

	void moveProcedure(std::shared_ptr<Piece> piece, int row, int col);
	void invalidateAllLegalMoves();
	void calculateAllLegalMoves();
	std::string checkGameEnd();
	std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
	bool move(std::shared_ptr<Piece> piece, int row, int col);
	const std::shared_ptr<Piece> isCheck();

	Board::Iterator begin();
	Board::Iterator end();
};
