#include "Knight.h"

#include <iostream>
#include <utility>

void Knight::calculatePossibleMoves(Board &board) {
  legalMoves.clear();
  auto boardSize = board.size();

  // Forward Right
      int row = getCurrentField().row + 2;
      int col = getCurrentField().col + 1;
      // check if move out of bounds
      if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
        // check if field is free
        if (!board[row][col]) {
          legalMoves.insert({row, col});
        }
        // check if move blocked by own figure
        else if (this->isWhite != board[row][col]->isWhite) {
          legalMoves.insert({row, col});
        }
      }

  // Forward Left
      row = getCurrentField().row + 2;
      col = getCurrentField().col - 1;
      // check if move out of bounds
      if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
        // check if field is free
        if (!board[row][col]) {
          legalMoves.insert({row, col});
        }
        // check if move blocked by own figure
        else if (this->isWhite != board[row][col]->isWhite) {
          legalMoves.insert({row, col});
        }
      }

  // Backward Left
      row = getCurrentField().row - 2;
      col = getCurrentField().col - 1;
      // check if move out of bounds
      if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
        // check if field is free
        if (!board[row][col]) {
          legalMoves.insert({row, col});
        }
        // check if move blocked by own figure
        else if (this->isWhite != board[row][col]->isWhite) {
          legalMoves.insert({row, col});
        }
      }

  // Backward Right
      row = getCurrentField().row - 2;
      col = getCurrentField().col + 1;
      // check if move out of bounds
      if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
        // check if field is free
        if (!board[row][col]) {
          legalMoves.insert({row, col});
        }
        // check if move blocked by own figure
        else if (this->isWhite != board[row][col]->isWhite) {
          legalMoves.insert({row, col});
        }
      }

  // Ŕight Forward
  row = getCurrentField().row + 1;
  col = getCurrentField().col + 2;
  // check if move out of bounds
  if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
    // check if field is free
    if (!board[row][col]) {
      legalMoves.insert({row, col});
    }
    // check if move blocked by own figure
    else if (this->isWhite != board[row][col]->isWhite) {
      legalMoves.insert({row, col});
    }
  }

  // Left Forward
  row = getCurrentField().row + 1;
  col = getCurrentField().col - 2;
  // check if move out of bounds
  if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
    // check if field is free
    if (!board[row][col]) {
      legalMoves.insert({row, col});
    }
    // check if move blocked by own figure
    else if (this->isWhite != board[row][col]->isWhite) {
      legalMoves.insert({row, col});
    }
  }

  // Ŕight backward
  row = getCurrentField().row - 1;
  col = getCurrentField().col + 2;
  // check if move out of bounds
  if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
    // check if field is free
    if (!board[row][col]) {
      legalMoves.insert({row, col});
    }
    // check if move blocked by own figure
    else if (this->isWhite != board[row][col]->isWhite) {
      legalMoves.insert({row, col});
    }
  }

  // Left backward
  row = getCurrentField().row - 1;
  col = getCurrentField().col - 2;
  // check if move out of bounds
  if ((-1 < row && row < boardSize && -1 < col && col < boardSize)) {
    // check if field is free
    if (!board[row][col]) {
      legalMoves.insert({row, col});
    }
    // check if move blocked by own figure
    else if (this->isWhite != board[row][col]->isWhite) {
      legalMoves.insert({row, col});
    }
  }
}