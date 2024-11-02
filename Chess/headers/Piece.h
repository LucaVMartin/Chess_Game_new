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
	  // index in 2-dim array

public:
	//member variables
	Coordinates currentField = { 0, 0 };
	const int pieceval;
	const bool isWhite = true;
	const unsigned id;
	bool gotMoved = false;
	std::unordered_set<Coordinates> posMoves{};

	//Consructor&Destructor
	Piece(int row = 0, int col = 0, bool isWhite = true, int pieceval_ = 0);
	virtual ~Piece() = default;

	//member functions
	const Coordinates getCurrentField() const;
	void setLegalMoves(Board& board);

	bool move(int row, int col, Board& board);

	virtual const std::string getName() const = 0;
	virtual void calculatePossibleMoves(Board& board) = 0;
	virtual void invalidateLegalMoves();
	
	//For queen, rook, bishop, King(king might be changed since it is unnecessary here)
	std::unordered_set<Coordinates> continuousMoveGenerator(
		Board& board, std::unordered_set<Coordinates> directions, int maxMoveLength = -1);
	void removeCheckedMoves(Board& board);
	virtual std::shared_ptr<Piece> clone() const = 0;
};
