#include "King.h"

#include <set>

#include "Board.h"

void King::removeMovesUnderCheck(Board& board) {
  for (auto piece : board) {
    if (piece->isWhite == this->isWhite || piece.get() == this) {
      continue;
    }
    for (auto move : legalMoves) {
      if (piece->getLegalMoves(board).count(move)) {
        legalMoves.erase(move);
      }
    }
  }
}

void King::calculatePossibleMoves(Board& board) {
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
  auto moves = continuousMoveGenerator(board, dirs, 1);
  legalMoves.merge(moves);
  removeMovesUnderCheck(board);
}