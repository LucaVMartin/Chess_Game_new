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
         std::array<std::shared_ptr<Piece>, 8>& operator[](int row);
         size_t size();
      // void set(int row, int col, std::shared_ptr<Piece>& piece) {
      //   board[row][col] = piece;
      // }

      // std::shared_ptr<Piece>& get(int row, int col) { return board[row][col]; }

         class Iterator {
             /* Iterates through the pieces contained in the board */
         public:
             BoardType& board;
             size_t i_row = 0;
             size_t i_col = 0;

             Iterator(BoardType& board);
             Iterator(BoardType& board, bool isEnd);

             Iterator& operator++();
             bool operator==(const Iterator& other) const;
             bool operator!=(const Iterator& other) const;
             std::shared_ptr<Piece> operator*();
         };
  Iterator begin();
  Iterator end();

};