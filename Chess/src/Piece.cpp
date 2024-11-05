#include "Piece.h"
#include <stdexcept>
#include "Board.h"

Piece::Piece(int row, int col, bool isWhite, int pieceval_)
	: isWhite(isWhite), id(counter++), currentField({ row, col }), pieceval(pieceval_) {}

const Coordinates Piece::getCurrentField() const { return currentField; }

void Piece::invalidateLegalMoves() {
	posMoves.clear();
}

void Piece::setLegalMoves(Board& board) {
	this->invalidateLegalMoves();
	calculatePossibleMoves(board);
	removeCheckedMoves(board);
}

bool Piece::move(int row, int col, Board& board) {
	if (this->posMoves.count({ row, col })) {
		if (!this->gotMoved) {
			this->gotMoved = true;
			board.firstMovedPiece = board[this->currentField.row][this->currentField.col];
			//this->justMadeFirstMove = true; //important for enpassant
		}
		else {
			board.firstMovedPiece = nullptr;
		}
		currentField = { row, col };
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
	//save pointer to current piece
	auto PtrCurrPiece = board[this->getCurrentField().row][this->getCurrentField().col];
	Coordinates kingpos;
	std::vector<Coordinates> illegalMoves; //vector to save the illegal moves
	board[this->getCurrentField().row][this->getCurrentField().col] = nullptr; //remove current piece
	bool enpassant = false;
	std::shared_ptr<Piece> saveEnpassantPiece;

	//loop over all possible moves of current piece
	for (auto& move : this->posMoves) {

		//Check if enpassant
		if (this->getName() == "pawn" && //check if it is a pawn
			this->getCurrentField().row == 3 + this->isWhite && //check if white piece is in 4th /black piece in 3rd row 
			!board[move.row][move.col] && //check if move field is empty
			this->getCurrentField().col != move.col)//check if it is a diagonal move
		{
			//save captured enpassant piece
			saveEnpassantPiece = board[move.row - (this->isWhite ? 1 : -1)][move.col];
			board[move.row - (this->isWhite ? 1 : -1)][move.col] = nullptr;
			enpassant = true;
		}
		std::shared_ptr<Piece> saveEnemyPiece = nullptr;
		if (board[move.row][move.col]) {
			saveEnemyPiece= board[move.row][move.col]; //save enemy piece which is removed if its a capture move
		}
		
		board[move.row][move.col] = PtrCurrPiece;//move current piece

		//find kingposition
		if (this->getName() == "king") {
			//if king is moved
			kingpos = move;
		}
		else {
			//set right king
			if (this->isWhite)
				kingpos = board.whiteKing->getCurrentField();
			else if (!this->isWhite)
				kingpos = board.blackKing->getCurrentField();
		}

		//loop over enemy pieces and check if they put check king. if they do => remove move from possible moves
		for (auto enemypiece : board) {
			if (enemypiece->isWhite != this->isWhite) { //only enemy pieces
				enemypiece->calculatePossibleMoves(board);
				for (auto& enemymove : enemypiece->posMoves) {
					if (enemymove == kingpos) {//this mean the move would lead to check => not legal
						if (enemypiece->getName() == "pawn" && enemypiece->currentField.col != kingpos.col && PtrCurrPiece->getName() == "king") {//sort out special case, where king couldnt move in front of a pawn
							illegalMoves.push_back(move);
						}
						else if (!(enemypiece->getName() == "pawn"))
						{
							illegalMoves.push_back(move); //move would lead to check
							goto exit;
						}

					}
				}
			}
			else {
				continue;
			}
		}
	exit:;
		if (saveEnemyPiece) {
			board[move.row][move.col] = saveEnemyPiece; //put enemy piece back
		}
		else {
			board[move.row][move.col] = nullptr;
		}

		if (enpassant) {
			board[move.row - (this->isWhite ? 1 : -1)][move.col] = saveEnpassantPiece; //put enpassant captured piece back in 
			enpassant = false;
		}
	}


	for (auto& ilmove : illegalMoves) {//erase illegal moves from list
		posMoves.erase(ilmove);
	}


	board[this->getCurrentField().row][this->getCurrentField().col] = PtrCurrPiece; //reset position of current piece
}