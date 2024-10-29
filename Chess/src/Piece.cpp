#include "Piece.h"
#include <stdexcept>
#include "Board.h"

Piece::Piece(int row, int col, bool isWhite)
    : isWhite(isWhite), id(counter++), currentField({row, col}) {}

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
  if (getLegalMoves(board).count({row, col})) {
    this->gotMoved = true;
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
void Piece::removeCheckedMoves(Board& board) {
    auto currPiece = board[this->getCurrentField().row][this->getCurrentField().col];
    Coordinates kingpos;
    std::vector<Coordinates> illegalMoves;
    board[this->getCurrentField().row][this->getCurrentField().col] = nullptr;
    for (auto& move: this->posMoves) {
        auto saveEnemyPiece = board[move.row][move.col];
        board[move.row][move.col] = currPiece;//move current piece
        for (auto piece : board) {//Find King
            if (piece->getName() == "king" && piece->isWhite == currPiece->isWhite) {
                if (currPiece->getName() == "king") {
                    kingpos = move;
                }
                else {
                    kingpos = piece->getCurrentField();
                }
                break;
            }
        }
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
    }
    for (auto& ilmove : illegalMoves) {//erase illegal moves from list
        posMoves.erase(ilmove);
    }
    board[this->getCurrentField().row][this->getCurrentField().col] = currPiece; //reset position of current piece
}