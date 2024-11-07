#include "Rook.h"

#include <unordered_set>

#include "Board.h"

std::unordered_set<Coordinates> Rook::calculatePossibleMoves(Board& board) {
	std::unordered_set<Coordinates> dirs = {
		{1, 0},   // up
		{-1, 0},  // down
		{0, 1},   // right
		{0, -1},  // left
	};
	auto moves = continuousMoveGenerator(board, dirs);
	return moves;
}