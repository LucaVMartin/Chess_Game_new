#include "Piece.h"
#include <stdexcept>
#include "Board.h"

Piece::Piece(int row, int col, bool isWhite)
    : isWhite(isWhite), id(counter++), currentField({row, col}) {}

const Coordinates Piece::getCurrentField() const { return currentField; }

void Piece::invalidateLegalMoves() {
  legalMoves.clear();
}

const std::unordered_set<Coordinates>& Piece::getLegalMoves(Board& board) {
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
      Coordinates move = {row, col};
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
