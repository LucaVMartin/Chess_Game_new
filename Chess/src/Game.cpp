#include "Game.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

#include <iostream>
Game::Game() : board() {
	this->calculateAllLegalMoves();
}

void Game::moveProcedure(std::shared_ptr<Piece> piece, int row, int col) {
	if (!this->move(piece, row, col)) return; //moves piece if possible
	this->resetJustMadeFirstMove(); //flag for possible enpassant
	this->isCheck(); //check for check
	this->nextTurn(); //toggles turn
	this->invalidateAllLegalMoves(); //deletes all possible moves of the pieces
	this->calculateAllLegalMoves(); //calculates legal moves for all pieces
}


void Game::calculateAllLegalMoves() {
	for (auto pieceFromBoard : board) {
		if (pieceFromBoard->isWhite == board.isWhiteTurn)
			pieceFromBoard->setLegalMoves(board);
	}
}

bool Game::checkGameEnd() {
	//Checkmate
	return false;
}

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
	auto startSquare = piece->getCurrentField();
	if (piece->isWhite != this->board.isWhiteTurn) return false;

	if (piece->move(row, col, board)) {  // change state in piece object
		// reflect changes on board

		//short castle
		if (piece->getName() == "king" && startSquare.col + 2 == col) {
			board[row][col + 1]->move(row, col - 1, board); //move rook as well
			board[row][col - 1] = board[row][col + 1];
			board[row][col + 1] = nullptr;
		}
		//long castle
		else if (piece->getName() == "king" && startSquare.col - 2 == col) {
			board[row][col - 2]->move(row, col + 1, board); //move rook as well
			board[row][col + 1] = board[row][col - 2];
			board[row][col - 2] = nullptr;
		}
		//Promotion
		else if (piece->getName() == "pawn" && (row == 7 || row == 0)) {
			board.Promotion.col = col;
			board.Promotion.row = row;
			this->promotion = true;
		}
		//enpassant
		else if (piece->getName() == "pawn" && //check if it is a pawn
			piece->getCurrentField().row == (piece->isWhite ? 5 : 2) && //check if white piece is in 5th /black piece in 2rd row 
			!board[row][col] && //check if move field is empty
			piece->getCurrentField().col != startSquare.col) { //check if diagonal move
			board[row - (piece->isWhite ? 1 : -1)][col] = nullptr;
		}

		board[startSquare.row][startSquare.col] = nullptr; //reset previous pos
		board[row][col] = piece; //put piece to new position
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
			//enemypiece->calculatePossibleMoves(board);
			for (auto& enemymove : enemypiece->posMoves) {
				if (enemymove == king->getCurrentField()) {//checks if move points to king => check
					dynamic_cast<King*>(king.get())->checked = true;
					return;
				}
			}
		}
	}
}

void Game::resetJustMadeFirstMove() {
	for (auto enemypiece : board) {
		if (enemypiece->isWhite != this->board.isWhiteTurn && enemypiece->getName() == "pawn") {
			enemypiece->justMadeFirstMove = false;
		}
	}
}

Board::Iterator Game::begin() { return board.begin(); }
Board::Iterator Game::end() { return board.end(); }