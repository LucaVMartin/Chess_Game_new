#include "Pawn.h"

void Pawn::calculatePossibleMoves(Board& board) {
	posMoves.clear();
	auto boardSize = board.size();
	auto sign =
		isWhite ? 1 : -1;  // white moves in different direction than black

	Coordinates movDir = { sign * 1, 0 };  // 1 Step
	// forward move
	for (auto i = 1; i < 3; ++i) {
		int row = getCurrentField().row + i * movDir.row;
		int col = getCurrentField().col + movDir.col;
		// move out of bounds
		if (!(-1 < row && row < boardSize && -1 < col && col < boardSize)) break;
		// move blocked
		if (board[row][col]) break;
		// 2step move
		if (i == 2 && this->gotMoved) break;
		posMoves.insert({ row, col });
	}

	// capture moves
	std::array<Coordinates, 2> captureDirs = { {
	   {sign , -1},  // capture left
	   {sign , 1}    // capture right
	} };
	for (auto dir : captureDirs) {
		int row = getCurrentField().row + dir.row;
		int col = getCurrentField().col + dir.col;
		// move out of bounds
		if (!(-1 < row && row < boardSize && -1 < col && col < boardSize)) continue;
		// check capture moves
		bool insertMove = false;
		//en passant
		if (this->gotMoved) {
			if (board[row - sign][col]) {//check if there is a piece
				if (board[row - sign][col]->isWhite != this->isWhite && board[row - sign][col]->getName() == "pawn") {
					if (board[row - sign][col]->justMadeFirstMove) {
						insertMove = true;
					}
				}
			}
		}

		if (board[row][col]) {
			if (board[row][col]->isWhite != this->isWhite) insertMove = true;
		};  // capture field is has opponent piece

		if (insertMove) posMoves.insert({ row, col });
	}

}