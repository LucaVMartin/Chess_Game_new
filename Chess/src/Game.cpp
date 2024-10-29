#include "Game.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

Game::Game() : board() {}

std::shared_ptr<Piece> Game::getPieceByCoordinates(int row, int col) {
	return board[row][col];
}

void Game::invalidateAllLegalMoves() {
	/*
	When a move is made, this potentially blocks other piece's
	legal moves. Thus they need to be invalidated.
	*/
	for (auto pieceFromBoard : board) {
		pieceFromBoard->invalidateLegalMoves();
	}
}


bool Game::move(std::shared_ptr<Piece> piece, int row, int col) {
	auto currentField = piece->getCurrentField();
	if (piece->isWhite != isWhiteTurn) return false;

	if (piece->move(row, col, board)) {  // change state in piece object
		// reflect changes on board
		board[currentField.row][currentField.col] = nullptr;
		board[row][col] = piece;

		isCheck();
		nextTurn();
		invalidateAllLegalMoves();
		return true;
	}
	return false;
}

const void Game::isCheck() {
	//find king
	std::shared_ptr<Piece> king;
	if (this->isWhiteTurn)
		king = board.blackKing;
	else
		king = board.whiteKing;

	for (auto enemypiece : board) {
		if (enemypiece->isWhite == this->isWhiteTurn) { //only enemy pieces
			enemypiece->calculatePossibleMoves(board);
			for (auto& enemymove : enemypiece->posMoves) {
				if (enemymove == king->getCurrentField()) {//this mean the move would lead to check => not legal
					dynamic_cast<King*>(king.get())->checked = true;
					return;
				}
			}
		}
	}
}

Board::Iterator Game::begin() { return board.begin(); }
Board::Iterator Game::end() { return board.end(); }