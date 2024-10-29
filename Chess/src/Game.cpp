#include "Game.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

void Game::initializeBoard() {
  // Pawns
  for (auto i = 0; i < 8; i++) {
    board[1][i] = std::make_shared<Pawn>(1, i, true);
    board[6][i] = std::make_shared<Pawn>(6, i, false);
  }
  // Rooks
  board[0][0] = std::make_shared<Rook>(0, 0, true);
  board[0][7] = std::make_shared<Rook>(0, 7, true);
  board[7][0] = std::make_shared<Rook>(7, 0, false);
  board[7][7] = std::make_shared<Rook>(7, 7, false);
  // Bishops
  board[0][2] = std::make_shared<Bishop>(0, 2, true);
  board[0][5] = std::make_shared<Bishop>(0, 5, true);
  board[7][2] = std::make_shared<Bishop>(7, 2, false);
  board[7][5] = std::make_shared<Bishop>(7, 5, false);
  // Queen
  board[0][3] = std::make_shared<Queen>(0, 3, true);
  board[7][3] = std::make_shared<Queen>(7, 3, false);
  // King
  board[0][4] = std::make_shared<King>(0, 4, true);
  board[7][4] = std::make_shared<King>(7, 4, false);
  whiteKing = board[0][4];
  blackKing = board[7][4];
  // Knights
  board[0][1] = std::make_shared<Knight>(0, 1, true);
  board[0][6] = std::make_shared<Knight>(0, 6, true);
  board[7][1] = std::make_shared<Knight>(7, 1, false);
  board[7][6] = std::make_shared<Knight>(7, 6, false);
}

Game::Game() : board{} { initializeBoard(); }

std::shared_ptr<Piece> Game::getPieceByCoordinates(int row, int col) {
  return board[row][col];
}

void Game::invalidateAllLegalMoves() {
  /*
  When a move is made, this potentially blocks other piece's
  legal moves. Thus they need to be invalidated.
  */
  for (auto pieceFromBoard : board) {
    pieceFromBoard->invalidateLegalMoves();
  }
}


bool Game::move(std::shared_ptr<Piece> piece, int row, int col) {
  auto currentField = piece->getCurrentField();
  if (piece->isWhite != isWhiteTurn) return false;

  if (piece->move(row, col, board)) {  // change state in piece object
    // reflect changes on board
    board[currentField.row][currentField.col] = nullptr;
    board[row][col] = piece;
    nextTurn();
    invalidateAllLegalMoves();
    return true;
  }
  return false;
}

Board::Iterator Game::begin() { return board.begin(); }
Board::Iterator Game::end() { return board.end(); }