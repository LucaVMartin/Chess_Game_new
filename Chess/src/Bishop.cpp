#include "Bishop.h"

#include <unordered_set>

#include "Board.h"

void Bishop::calculatePossibleMoves(Board& board) {
  std::unordered_set<Coordinates> dirs = {
      {1, 1},    // diagonal up right
      {1, -1},   // diagonal up left
      {-1, -1},  // diagonal down left
      {-1, 1},   // diagonal down right
  };
  auto moves = continuousMoveGenerator(board, dirs);
  legalMoves.merge(moves);
}