#pragma once

#include <memory>
#include <unordered_set>
#include <string>

struct Coordinates {
  int row;
  int col;
  Coordinates(int r, int c) : row(r), col(c) {}
  Coordinates() : row(0), col(0) {}
  //needed since unordered set is created with this struct
  bool operator==(const Coordinates& other) const {
      return row == other.row && col == other.col;
  }
};
//needed since unordered set is created with Coordinates. This is the hash function
namespace std {
    template<>
    struct hash<Coordinates> {
        size_t operator()(const Coordinates& coord) const {
            return hash<int>()(coord.row) ^ (hash<int>()(coord.col) << 1);
        }
    };
}

class Board;
class Piece {
private:
    static inline unsigned counter = 0; //to set the id of the piece (counter gets incremented when a Piece is constructed)
    Coordinates currentField = { 0, 0 };  // index in 2-dim array

public:
  const bool isWhite = true;
  const unsigned id;
  bool gotMoved = false;
  bool justMadeFirstMove = false;
  std::unordered_set<Coordinates> posMoves{};
  Piece(int row = 0, int col = 0, bool isWhite = true);
  virtual ~Piece() = default;

  const Coordinates getCurrentField() const;
  const std::unordered_set<Coordinates>& getLegalMoves(Board& board);
  virtual const std::string getName() const = 0;

  bool move(int row, int col, Board& board);

  
  virtual void invalidateLegalMoves();

  virtual void calculatePossibleMoves(Board& board) = 0;
  //For queen, rook, bishop, King(king might be changed since it is unnecessary here)
  void removeCheckedMoves(Board& board);
  std::unordered_set<Coordinates> continuousMoveGenerator(
      Board& board, std::unordered_set<Coordinates> directions, int maxMoveLength = -1);
};
