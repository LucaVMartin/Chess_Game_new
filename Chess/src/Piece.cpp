#include "Piece.h"
#include <stdexcept>
#include "Board.h"

Piece::Piece(int row, int col, bool isWhite)
	: isWhite(isWhite), id(counter++), currentField({ row, col }) {}

const Coordinates Piece::getCurrentField() const { return currentField; }

void Piece::invalidateLegalMoves() {
	posMoves.clear();
}

const std::unordered_set<Coordinates>& Piece::getLegalMoves(Board& board) {
	calculatePossibleMoves(board);
	removeCheckedMoves(board);
	return posMoves;
}

bool Piece::move(int row, int col, Board& board) {
	if (getLegalMoves(board).count({ row, col })) {
		if (!this->gotMoved) {
			this->gotMoved = true;
			this->justMadeFirstMove = true; //important for enpassant
		}

		currentField = { row, col };
		invalidateLegalMoves();
		return true;
	}
	return false;
}

std::unordered_set<Coordinates> Piece::continuousMoveGenerator(
	Board& board, std::unordered_set<Coordinates> directions, int maxMoveLength) {
	int length = static_cast<int>(board.size());
	if (maxMoveLength > -1 && maxMoveLength < length) {
		length = maxMoveLength + 1;
	}
	std::unordered_set<Coordinates> moves;
	//loop over all valid directions for the piece
	for (auto& dir : directions) {
		//check how many steps are possible in that direction
		for (auto i = 1; i < length; ++i) {
			int row = this->getCurrentField().row + i * dir.row;
			int col = this->getCurrentField().col + i * dir.col;
			//break if move would be out of bounds of the board
			if (row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
				break;
			}
			Coordinates move = { row, col };
			//Insert to possible moves if there is no piece 
			if (!board[row][col]) {
				moves.insert(move);
			}
			//Insert move if there is an enemy piece in the way, break then
			else if (!(board[row][col]->isWhite == isWhite)) {
				moves.insert(move);
				break;
			} //If there is a white piece in the way, break
			else {
				break;
			}
		}
	}
	return moves;
}
void Piece::removeCheckedMoves(Board& board) {
	auto currPiece = board[this->getCurrentField().row][this->getCurrentField().col];
	Coordinates kingpos;
	std::vector<Coordinates> illegalMoves;
	board[this->getCurrentField().row][this->getCurrentField().col] = nullptr;
	bool enpassant = false;
	std::shared_ptr<Piece> saveEnpassantPiece;
	for (auto& move : this->posMoves) {

		//Check if enpassant
		if (currPiece->getName() == "pawn" && //check if it is a pawn
			currPiece->getCurrentField().row == 3 + currPiece->isWhite && //check if white piece is in 4th /black piece in 3rd row 
			!board[move.row][move.col] && //check if move field is empty
			currPiece->getCurrentField().col != move.col)//check if it is a diagonal move
		{
			//save captured piece
			saveEnpassantPiece = board[move.row - (currPiece->isWhite ? 1 : -1)][move.col];
			board[move.row - (currPiece->isWhite ? 1 : -1)][move.col] = nullptr;
			enpassant = true;
		}

		auto saveEnemyPiece = board[move.row][move.col];
		board[move.row][move.col] = currPiece;//move current piece
		//Set kingposition
		if (currPiece->getName() == "king") {
			//if king is moved
			kingpos = move;
		}
		else {
			//set right king
			if (currPiece->isWhite)
				kingpos = board.whiteKing->getCurrentField();
			else if (!currPiece->isWhite)
				kingpos = board.blackKing->getCurrentField();
		}
		//loop over enemy pieces and check if they put check king. if they do => remove move from possible moves
		for (auto enemypiece : board) {
			if (enemypiece->isWhite != currPiece->isWhite) { //only enemy pieces
				enemypiece->calculatePossibleMoves(board);
				for (auto& enemymove : enemypiece->posMoves) {
					if (enemymove == kingpos) {//this mean the move would lead to check => not legal
						illegalMoves.push_back(move);
						goto exit;
					}
				}
			}
			else {
				continue;
			}
		}
	exit:;
		board[move.row][move.col] = saveEnemyPiece;
		if (enpassant) {
			board[move.row - (currPiece->isWhite ? 1 : -1)][move.col] = saveEnpassantPiece; //put captured piece back in 
			enpassant = false;
		}
	}


	for (auto& ilmove : illegalMoves) {//erase illegal moves from list
		posMoves.erase(ilmove);
	}


	board[this->getCurrentField().row][this->getCurrentField().col] = currPiece; //reset position of current piece
}