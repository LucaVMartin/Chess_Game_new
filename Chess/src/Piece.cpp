#include "Piece.h"

#include <stdexcept>

#include "Board.h"

Piece::Piece(int row, int col, bool isWhite)
    : isWhite(isWhite), id(counter++), currentField({row, col}) {}

Coordinates Piece::getCurrentField() const { return currentField; }

void Piece::invalidateLegalMoves(std::shared_ptr<Piece> pieceThatMoved) {
  legalMoves.clear();
}

const std::set<Coordinates>& Piece::getLegalMoves(Board& board) {
  if (legalMoves.empty()) {
    // if (!board)
    //   std::runtime_error("board is nullptr. please provide a board
    //   reference");
    calculatePossibleMoves(board);
  }
  return legalMoves;
}

bool Piece::move(int row, int col, Board& board) {
  if (getLegalMoves(board).count({row, col})) {
    gotMoved = true;
    currentField = {row, col};
    invalidateLegalMoves();
    return true;
  }
  return false;
}

std::set<Coordinates> Piece::continuousMoveGenerator(
    Board& board, std::set<Coordinates> directions, int maxMoveLength) {
  int length = board.size();
  if (maxMoveLength > -1 && maxMoveLength < length) {
    length = maxMoveLength + 1;
  }
  std::set<Coordinates> moves;
  for (auto dir : directions) {
    for (auto i = 1; i < length; ++i) {
      int row = getCurrentField().row + i * dir.row;
      int col = getCurrentField().col + i * dir.col;

      if (row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
        break;
      }
      Coordinates move = {row, col};
      moves.insert(move);
      if (board[row][col]) {
        if (board[row][col]->isWhite == isWhite) {
          moves.erase(move);
        }
        break;
      }
    }
  }
  return moves;
}
