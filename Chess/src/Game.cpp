#include "Game.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

#include <iostream>
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


bool Game::move(std::shared_ptr<Piece> piece, int row, int col) { //returns true if promotion
	auto currentField = piece->getCurrentField();
	if (piece->isWhite != this->board.isWhiteTurn) return false;

	if (piece->move(row, col, board)) {  // change state in piece object
		// reflect changes on board

		//short castle
		if (piece->getName() == "king" && currentField.col + 2 == col) {
			board[row][col + 1]->move(row, col - 1, board); //move rook as well
			board[row][col - 1] = board[row][col + 1];
			board[row][col + 1] = nullptr;
		}
		//long castle
		else if (piece->getName() == "king" && currentField.col - 2 == col) {
			board[row][col - 2]->move(row, col + 1, board); //move rook as well
			board[row][col + 1] = board[row][col - 2];
			board[row][col - 2] = nullptr;
		}
		//Promotion
		else if (piece->getName() == "pawn" && (row == 7 || row == 0)) {
			board.Promotion.col = col;
			board.Promotion.row = row;
			promotion = true;
		}

		board[currentField.row][currentField.col] = nullptr; //reset previous pos
		board[row][col] = piece; //put piece to new position

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
	if (this->board.isWhiteTurn) {
		king = board.blackKing;
		dynamic_cast<King*>(board.whiteKing.get())->checked = false;
	}
	else {
		king = board.whiteKing;
		dynamic_cast<King*>(board.blackKing.get())->checked = false;
	}

	for (auto enemypiece : board) {
		if (enemypiece->isWhite == this->board.isWhiteTurn) { //only enemy pieces
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