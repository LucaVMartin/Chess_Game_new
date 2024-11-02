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
	if (!this->board.move(piece, row, col)) return; //moves piece if possible
	auto attackPiece = this->isCheck(); //check for check
	this->nextTurn(); //toggles turn
	this->invalidateAllLegalMoves(); //deletes all possible moves of the pieces
	this->calculateAllLegalMoves(); //calculates legal moves for all pieces
	auto test = this->checkGameEnd(attackPiece);
	std::cout << test << std::endl;
}


void Game::calculateAllLegalMoves() {
	for (auto pieceFromBoard : board) {
		if (pieceFromBoard->isWhite == board.isWhiteTurn)
			pieceFromBoard->setLegalMoves(board);
	}
}

std::string Game::checkGameEnd(std::shared_ptr<Piece> attackPiece) {
	auto king = this->board.isWhiteTurn ? dynamic_cast<King*>(board.whiteKing.get()) : dynamic_cast<King*>(board.blackKing.get());
	bool posmoves = false;
	std::vector<Coordinates> squaresToBlock;

	//check if there are possible moves (stalemate/checkmate)
	for (auto ownPiece : board) {
		if (ownPiece->isWhite == board.isWhiteTurn) {
			if (!ownPiece->posMoves.size() == 0) {
				posmoves = true;
				break;
			}
		}
	}
	if (!posmoves) {
		if (king->checked)
			return std::string("Checkmate. ") + (king->isWhite ? "Black" : "White") + " wins!";//checkmate
		else
			return "Stalemate!";  // stalemate if no moves available
	}

	return "";
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


const std::shared_ptr<Piece> Game::isCheck() {
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
				if (enemymove == king->getCurrentField()) {//checks if move points to king => check
					dynamic_cast<King*>(king.get())->checked = true;
					return enemypiece;
				}
			}
		}
	}
	return nullptr;
}

Board::Iterator Game::begin() { return board.begin(); }
Board::Iterator Game::end() { return board.end(); }