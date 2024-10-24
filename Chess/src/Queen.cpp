#include "Queen.h"

#include <set>

#include "Board.h"

void Queen::calculatePossibleMoves(Board& board) {
  std::set<Coordinates> dirs = {
      {1, 1},    // diagonal up right
      {1, -1},   // diagonal up left
      {-1, -1},  // diagonal down left
      {-1, 1},   // diagonal down right
      {1, 0},    // up
      {-1, 0},   // down
      {0, 1},    // right
      {0, -1},   // left
  };
  auto moves = continuousMoveGenerator(board, dirs);
  legalMoves.merge(moves);
}