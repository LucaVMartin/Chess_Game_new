#include "Board.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

Board::Board() {
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
//copy constructor
Board::Board(const Board& other)
	: Promotion(other.Promotion), promotion(other.promotion), isWhiteTurn(other.isWhiteTurn) {
	// Deep copy of the board
	for (size_t row = 0; row < board.size(); ++row) {
		for (size_t col = 0; col < board[row].size(); ++col) {
			if (other.board[row][col]) {
				// Create a new piece based on the type of the original piece
				if (other.board[row][col]->getName() == "pawn") {
					board[row][col] = std::make_shared<Pawn>(*dynamic_cast<Pawn*>(other.board[row][col].get()));
				}
				else if (other.board[row][col]->getName() == "rook") {
					board[row][col] = std::make_shared<Rook>(*dynamic_cast<Rook*>(other.board[row][col].get()));
				}
				else if (other.board[row][col]->getName() == "knight") {
					board[row][col] = std::make_shared<Knight>(*dynamic_cast<Knight*>(other.board[row][col].get()));
				}
				else if (other.board[row][col]->getName() == "bishop") {
					board[row][col] = std::make_shared<Bishop>(*dynamic_cast<Bishop*>(other.board[row][col].get()));
				}
				else if (other.board[row][col]->getName() == "queen") {
					board[row][col] = std::make_shared<Queen>(*dynamic_cast<Queen*>(other.board[row][col].get()));
				}
				else if (other.board[row][col]->getName() == "king") {
					board[row][col] = std::make_shared<King>(*dynamic_cast<King*>(other.board[row][col].get()));
					if (board[row][col]->isWhite) {
						whiteKing = board[row][col];
					}
					else if (!(board[row][col]->isWhite)) {
						blackKing = board[row][col];
					}
				}
			}
		}
	}
}

bool Board::move(std::shared_ptr<Piece> piece, int row, int col) {
	auto startSquare = piece->getCurrentField();
	if (piece->isWhite != this->isWhiteTurn) return false;

	if (piece->move(row, col, *this)) {  // change state in piece object
		// reflect changes on board

		//short castle
		if (piece->getName() == "king" && startSquare.col + 2 == col) {
			board[row][col + 1]->move(row, col - 1, *this); //move rook as well
			board[row][col - 1] = board[row][col + 1];
			board[row][col + 1] = nullptr;
		}
		//long castle
		else if (piece->getName() == "king" && startSquare.col - 2 == col) {
			board[row][col - 2]->move(row, col + 1, *this); //move rook as well
			board[row][col + 1] = board[row][col - 2];
			board[row][col - 2] = nullptr;
		}
		//Promotion
		else if (piece->getName() == "pawn" && (row == 7 || row == 0)) {
			this->Promotion.col = col;
			this->Promotion.row = row;
			this->promotion = true;
		}
		//enpassant
		else if (piece->getName() == "pawn" && //check if it is a pawn
			piece->getCurrentField().row == (piece->isWhite ? 5 : 2) && //check if white piece is in 5th /black piece in 2rd row 
			!board[row][col] && //check if move field is empty
			piece->getCurrentField().col != startSquare.col) { //check if diagonal move
			board[row - (piece->isWhite ? 1 : -1)][col] = nullptr;
		}

		board[startSquare.row][startSquare.col] = nullptr; //reset previous pos
		board[row][col] = piece; //put piece to new position
		return true;
	}
	return false;
}

void Board::createPromotionPiece(std::string pieceName) {
	if (pieceName == "queen") {
		board[Promotion.row][Promotion.col] = std::make_shared<Queen>(Promotion.row, Promotion.col, !this->isWhiteTurn);
	}
	else if (pieceName == "knight") {
		board[Promotion.row][Promotion.col] = std::make_shared<Knight>(Promotion.row, Promotion.col, !this->isWhiteTurn);
	}
	else if (pieceName == "rook") {
		board[Promotion.row][Promotion.col] = std::make_shared<Rook>(Promotion.row, Promotion.col, !this->isWhiteTurn);
	}
	else if (pieceName == "bishop") {
		board[Promotion.row][Promotion.col] = std::make_shared<Bishop>(Promotion.row, Promotion.col, !this->isWhiteTurn);
	}
	board[Promotion.row][Promotion.col]->gotMoved = true;
}

std::array<std::shared_ptr<Piece>, 8>& Board::operator[](int row) {
	return board[row];
}

size_t Board::size() { return board.size(); }

Board::Iterator::Iterator(BoardType& board) : board(board) {
	if (operator*() == nullptr) {
		++(*this); //so it points to the first piece on the board
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