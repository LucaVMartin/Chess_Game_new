#pragma once

#include <memory>
#include <set>
#include <string>

struct Coordinates {
  int row;
  int col;

  bool operator<(const Coordinates& other) const {
    if (row != other.row) return row < other.row;
    return col < other.col;
  }
};

class Board;
class Piece {
private:
    static inline unsigned counter = 0;
    Coordinates currentField = { 0, 0 };  // index in 2-dim array

 public:
  const bool isWhite = true;
  const unsigned id;
  bool gotMoved = false;

  Piece(int row = 0, int col = 0, bool isWhite = true);
  virtual ~Piece() = default;

  const Coordinates getCurrentField() const;
  const std::set<Coordinates>& getLegalMoves(Board& board);
  virtual const std::string getName() const = 0;

  bool move(int row, int col, Board& board);

  
  virtual void invalidateLegalMoves();
  /* nullptr means one self has moved TODO potential
             optmization */

  std::set<Coordinates> legalMoves{};
  virtual void calculatePossibleMoves(Board& board) = 0;
  std::set<Coordinates> continuousMoveGenerator(
      Board& board, std::set<Coordinates> directions, int maxMoveLength = -1);
};
