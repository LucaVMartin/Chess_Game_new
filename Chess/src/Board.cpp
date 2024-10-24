#include "Board.h"

Board::Iterator Board::begin() { return Board::Iterator(board); }

Board::Iterator Board::end() { return Board::Iterator(board, true); }