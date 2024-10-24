#pragma once

#include <array>
#include <iterator>
#include <memory>

#include "Piece.h"

using BoardType = std::array<std::array<std::shared_ptr<Piece>, 8>, 8>;
class Board {
 private:
  BoardType board;

 public:
  std::array<std::shared_ptr<Piece>, 8>& operator[](int row) {
    return board[row];
  }

  // void set(int row, int col, std::shared_ptr<Piece>& piece) {
  //   board[row][col] = piece;
  // }

  // std::shared_ptr<Piece>& get(int row, int col) { return board[row][col]; }

  class Iterator
      : std::iterator<std::input_iterator_tag, std::shared_ptr<Piece>, int,
                      std::shared_ptr<Piece>, Piece> {
    /* Iterates through the pieces contained in the board */
   public:
    BoardType& board;
    size_t i_row = 0;
    size_t i_col = 0;

    Iterator(BoardType& board) : board(board) {
      if (operator*() == nullptr) {
        ++(*this);
      }
    };
    Iterator(BoardType& board, bool isEnd)
        : board(board), i_row(board.size()){};

    std::shared_ptr<Piece> operator*() {
      if (i_row < board.size()) {
        return board[i_row][i_col];
      }
      return nullptr;
    }

    Iterator& operator++() {
      do {
        if (i_col == board.size() - 1) {
          ++i_row;
          i_col = 0;
        } else {
          ++i_col;
        }
      } while (i_row < board.size() && board[i_row][i_col] == nullptr);
      return *this;
    }

    bool operator==(Iterator& other) {
      if (&board == &other.board) {
        if (i_row == other.i_row && i_col == other.i_col) return true;
      }
      return false;
    }

    bool operator!=(Iterator& other) { return !operator==(other); }
  };

  Iterator begin();
  Iterator end();
  int size() { return board.size(); }
};
