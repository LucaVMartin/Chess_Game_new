#include "King.h"

#include <unordered_set>
#include "Board.h"


std::unordered_set<Coordinates> King::calculatePossibleMoves(Board& board) {
	std::unordered_set<Coordinates> dirs = {
		{1, 1},    // diagonal up right
		{1, -1},   // diagonal up left
		{-1, -1},  // diagonal down left
		{-1, 1},   // diagonal down right
		{1, 0},    // up
		{-1, 0},   // down
		{0, 1},    // right
		{0, -1},   // left
	};
	auto moves = continuousMoveGenerator(board, dirs, 1);

	if (this->isWhite == board.isWhiteTurn && //Only castle if it is your turn
		!this->gotMoved && //king not moved
		!this->checked)//king not checked
	{
		//short castle
		if (
			board[this->getCurrentField().row][this->getCurrentField().col + 3] && //piece is there 
			board[this->getCurrentField().row][this->getCurrentField().col + 3]->getName() == "rook" && //piece is rook
			!board[this->getCurrentField().row][this->getCurrentField().col + 3]->gotMoved && //rook not moved
			!board[this->getCurrentField().row][this->getCurrentField().col + 1] && //Field next to king empty
			!board[this->getCurrentField().row][this->getCurrentField().col + 2] //Field 2 next to king empty
			) {
			bool castleFlag = true;
			for (auto enemypiece : board) {
				if (enemypiece->isWhite != this->isWhite) { //only enemy pieces
					auto enemymoves = enemypiece->calculatePossibleMoves(board);
					for (auto& enemymove : enemymoves) { //iterate over possible moves of enemy pieces
						if (enemymove == Coordinates(this->getCurrentField().row, this->getCurrentField().col + 1) ||
							enemymove == Coordinates(this->getCurrentField().row, this->getCurrentField().col + 2)) {//one of the squares is attacked
							castleFlag = false;
							break;
						}
					}
				}
				if (!castleFlag) break;
			}
			//Insert castle move
			if (castleFlag) {
				moves.insert(Coordinates(this->getCurrentField().row, this->getCurrentField().col + 2));
			}
		}

		//long castle
		if (
			board[this->getCurrentField().row][this->getCurrentField().col - 4] && //piece is there 
			board[this->getCurrentField().row][this->getCurrentField().col - 4]->getName() == "rook" && //piece is rook
			!board[this->getCurrentField().row][this->getCurrentField().col - 4]->gotMoved && //rook not moved
			!board[this->getCurrentField().row][this->getCurrentField().col - 1] && //Field next to king empty
			!board[this->getCurrentField().row][this->getCurrentField().col - 2] &&//Field 2 next to king empty
			!board[this->getCurrentField().row][this->getCurrentField().col - 3] //Field 3 next to king empty
			) {
			bool castleFlag = true;
			for (auto enemypiece : board) {
				if (enemypiece->isWhite != this->isWhite) { //only enemy pieces
					auto enemymoves = enemypiece->calculatePossibleMoves(board);
					for (auto& enemymove : enemymoves) {
						if (enemymove == Coordinates(this->getCurrentField().row, this->getCurrentField().col - 1) ||
							enemymove == Coordinates(this->getCurrentField().row, this->getCurrentField().col - 2)) {//one of the squares is not attacked
							castleFlag = false;
							break;
						}
					}
				}
				if (!castleFlag) break;
			}
			//Insert castle move
			if (castleFlag) {
				moves.insert(Coordinates(this->getCurrentField().row, this->getCurrentField().col - 2));
			}
		}
	}
	return moves;
}