#include "Board.h"

std::array<std::shared_ptr<Piece>, 8>& Board::operator[](int row) {
         return board[row];
     }

size_t Board::size() { return board.size();}

Board::Iterator::Iterator(BoardType& board) : board(board) {
    if (operator*() == nullptr) {
        ++(*this);
    }
}

Board::Iterator::Iterator(BoardType& board, bool isEnd)
    : board(board), i_row(board.size()) {}

std::shared_ptr<Piece> Board::Iterator::operator*() {
    if (i_row < board.size()) {
        return board[i_row][i_col];
    }
    return nullptr;
}

Board::Iterator& Board::Iterator::operator++() {
    do {
        if (i_col == board.size() - 1) {
            ++i_row;
            i_col = 0;
        }
        else {
            ++i_col;
        }
    } while (i_row < board.size() && board[i_row][i_col] == nullptr);
    return *this;
}

bool Board::Iterator::operator==(const Iterator& other) const {
    return &board == &other.board && i_row == other.i_row && i_col == other.i_col;
}

bool Board::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}


Board::Iterator Board::begin() { return Board::Iterator(board); }

Board::Iterator Board::end() { return Board::Iterator(board, true); }