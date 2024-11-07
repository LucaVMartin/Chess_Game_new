#include "Engine.h"
#include "UI.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
void Engine::visualizeBoard(sf::RenderWindow& windowEngine, Board& board) {
	if (!windowEngine.isOpen()) {
		windowEngine.create(sf::VideoMode(800, 800), "TestWindow");
		windowEngine.setFramerateLimit(30);
	}

	UI ui{ &windowEngine };

	windowEngine.clear();
	ui.drawBoardtest(board, windowEngine);
	windowEngine.display();
	sf::sleep(sf::milliseconds(0));
}

int Engine::evalPosition(Board& board) {
	bool check = false;
	if (this->testing) {
		check = updateCheckStatus(board);
		if (check) {
			checkctr++;
		}
	}
	//check for stalemate/checkmate: 
	if (GameEndCheck(board)) {
		//check for checkmate
		if (check || updateCheckStatus(board)) {
			checkmatectr++;
			return (board.isWhiteTurn ? (-INT_MAX) : (INT_MAX)); //return high/low value if checkmate
		}
		else
		{
			return 0; //stalemate
		}
	}
	//Evaluate position
	int totalval = 0;
	for (auto piece : board) {
		if (piece->isWhite)
			totalval += piece->pieceval;
		else
			totalval -= piece->pieceval;
	}
	return ((board.isWhiteTurn ? 1 : -1) * totalval);
}

int Engine::giveNumPos(int depth) {
	int numpos;
	switch (depth) {
	case 1: numpos = 20; break;
	case 2: numpos = 400; break;
	case 3: numpos = 8902; break;
	case 4: numpos = 197281; break;
	case 5: numpos = 4865609; break;
	case 6: numpos = 119060324; break;
	default: numpos = 0; break; // Handle invalid depths
	}
	return numpos;
}

evalMove Engine::createTree(Board& board, int depth) {
	int flagGameEnd = 0; //flag to check for game ends before leaf node
	static int originalDepth = -1;
	static int numpos = 0;
	static int lastPrintedPercentage = -1;
	if (this->testing && originalDepth == -1) {
		originalDepth = depth;
		numpos = giveNumPos(originalDepth);
	}
	if (depth == 0) {
		if (this->testing)
		{
			positionctr++;
			//visualizeBoard(wind, board);
			int progressPercentage = (positionctr * 100) / numpos;
			// Check if the progress percentage is a multiple of 5 and hasn't been printed yet
			if (progressPercentage % 5 == 0 && progressPercentage != lastPrintedPercentage) {
				std::cout << "Progress: " << progressPercentage << "%" << std::endl;
				lastPrintedPercentage = progressPercentage; // Update last printed percentage
			}
		}
		return evalMove(Move(), this->evalPosition(board)); //return board value 
	}
	else {
		// calculate all possible moves
		for (auto pieceFromBoard : board) {
			if (pieceFromBoard->isWhite == board.isWhiteTurn)
				pieceFromBoard->setLegalMoves(board);
		}
		;
		evalMove best;
		best.score = INT_MIN;
		//Board minboard;
		for (auto piece : board) {
			if (piece->isWhite == board.isWhiteTurn) {
				for (auto& move : piece->posMoves) {
					Board newBoard = board;
					if (this->testing && depth == 1) {
						this->countCaptures(board, move);//count captures for testing (only at leaf nodes)
					}
					newBoard.move(newBoard[piece->getCurrentField().row][piece->getCurrentField().col], move.row, move.col);
					newBoard.isWhiteTurn = !newBoard.isWhiteTurn;
					//visualize					
					//visualizeBoard(this->wind, newBoard);

					auto result = createTree(newBoard, depth - 1);
					result.score = -result.score;


					if (result.score > best.score) {
						best.score = result.score;
						best.move.moveCoords = move;
						best.move.pieceCoords = piece->getCurrentField();
					}
					flagGameEnd++; // to check if there is no legal move;
				}
			}
		}
		if (flagGameEnd == 0) { //no legal move 
			auto check = updateCheckStatus(board);
			board.isWhiteTurn = !board.isWhiteTurn;
			//visualizeBoard(wind, board);
			if (check) {
				return evalMove(Move(), -(INT_MAX));
			}
			else {
				std::cout << "stalemate before leaf node" << std::endl;
				return evalMove(Move(), 0);
			}
		}
		return best;
	}
}

bool Engine::updateCheckStatus(Board& board)
{
	std::shared_ptr<Piece> king;

	// Determine which king to check
	if (board.isWhiteTurn) {
		king = board.whiteKing;
		auto blackKingPtr = dynamic_cast<King*>(board.blackKing.get());
		if (!blackKingPtr) {
			throw std::runtime_error("Error: Black king is not a valid King or is null.");
		}
		blackKingPtr->checked = false; // Set the white king as not checked
	}
	else {
		king = board.blackKing;
		auto whiteKingPtr = dynamic_cast<King*>(board.whiteKing.get());
		if (!whiteKingPtr) {
			throw std::runtime_error("Error: White king is not a valid King or is null.");
		}
		whiteKingPtr->checked = false; // Set the black king as not checked
	}

	// Calculate possible moves of enemy pieces
	for (auto pieceFromBoard : board) {
		if (pieceFromBoard) {
			if (pieceFromBoard->isWhite != board.isWhiteTurn) {
				pieceFromBoard->setLegalMoves(board);
			}
		}
		else {
			throw std::runtime_error("Error: Piece from board is null.");
		}
	}

	// Check if the king is in check
	for (const auto& pieceFromBoard : board) {
		if (pieceFromBoard) {
			if (pieceFromBoard->isWhite != board.isWhiteTurn) {
				for (const auto& move : pieceFromBoard->posMoves) {
					if (king) {
						if (move == king->getCurrentField()) { // If a move points to the king
							dynamic_cast<King*>(king.get())->checked = true; // Set the king as checked
							return true; // Exit the function once we find the king is in check
						}
					}
					else {
						throw std::runtime_error("Error: King pointer is null.");
					}
				}
			}
		}
		else {
			throw std::runtime_error("Error: Piece from board is null.");
		}
	}
	return false;
}


void Engine::countCaptures(Board& board, Coordinates move)
{
	//count captures
	if (board[move.row][move.col]) {
		this->capturectr++; //does enpassent count as well?
	}
}

bool Engine::GameEndCheck(Board& board)
{
	auto king = board.isWhiteTurn ? dynamic_cast<King*>(board.whiteKing.get()) : dynamic_cast<King*>(board.blackKing.get());
	bool posmoves = false;

	//check if there are possible moves (stalemate/checkmate)
	for (auto ownPiece : board) {
		if (ownPiece->isWhite == board.isWhiteTurn) {
			ownPiece->setLegalMoves(board); //calculate legal moves for own piece
			if (!ownPiece->posMoves.size() == 0) {
				return false;//if there is a legal move, game is not over
			}
		}
	}
	return true;
}


evalMove Engine::alphabeta(Board& board, int depth, int alpha, int beta) {
	int flagGameEnd = 0; //flag to check for game ends before leaf node
	static int originalDepth = -1;
	static int numpos = 0;
	static int lastPrintedPercentage = -1;
	if (this->testing && originalDepth == -1) {
		originalDepth = depth;
		numpos = giveNumPos(originalDepth);
	}
	if (depth == 0) {
		if (this->testing)
		{
			positionctr++;
			//visualizeBoard(wind, board);
			int progressPercentage = (positionctr * 100) / numpos;
			// Check if the progress percentage is a multiple of 5 and hasn't been printed yet
			if (progressPercentage % 5 == 0 && progressPercentage != lastPrintedPercentage) {
				std::cout << "Progress: " << progressPercentage << "%" << std::endl;
				lastPrintedPercentage = progressPercentage; // Update last printed percentage
			}
		}
		return evalMove(Move(), this->evalPosition(board)); //return board value 
	}
	else {
		// calculate all possible moves
		auto allMoves = moveGenerator(board);
		evalMove best;
		best.score = INT_MIN;
		//Board minboard;
		for (const auto& move : allMoves) {
			Board newBoard = board;
			if (this->testing && depth == 1) {
				this->countCaptures(board, move.moveCoords);//count captures for testing (only at leaf nodes)
			}
			//visualizeBoard(this->wind, newBoard);
			newBoard.move(newBoard[move.pieceCoords.row][move.pieceCoords.col], move.moveCoords.row, move.moveCoords.col);
			if (move.isPromotion) {
				createPromotionPiece(newBoard, move);
			}
			newBoard.isWhiteTurn = !newBoard.isWhiteTurn;
			//visualize					
			//visualizeBoard(this->wind, newBoard);

			auto result = alphabeta(newBoard, depth - 1, -beta, -alpha);
			result.score = -result.score;


			if (result.score > best.score) {
				best.score = result.score;
				best.move = move;
			}
			alpha = std::max(alpha, result.score);
			if (alpha >= beta) {
				return best;
			}
			flagGameEnd++; // to check if there is no legal move;
		}
		if (flagGameEnd == 0) { //no legal move 
			auto check = updateCheckStatus(board);
			board.isWhiteTurn = !board.isWhiteTurn;
			//visualizeBoard(wind, board);
			if (check) {
				return evalMove(Move(), -(INT_MAX));
			}
			else {
				std::cout << "stalemate before leaf node" << std::endl;
				return evalMove(Move(), 0);
			}
		}
		return best;
	}
}

Move Engine::calculateEngineMove(Board& board, int depth)
{
	if (!board.isWhiteTurn) {
		std::cout << "Calculating move..." << std::endl;
		auto bestMove = this->alphabeta(board, depth, -INT_MAX, INT_MAX);
		auto perfMove = bestMove.move;

		if (board[perfMove.pieceCoords.row][perfMove.pieceCoords.col]) {
			std::cout << "Put the " << board[perfMove.pieceCoords.row][perfMove.pieceCoords.col]->getName() <<
				" on " << perfMove.pieceCoords.row << " - " << perfMove.pieceCoords.col << " to " <<
				perfMove.moveCoords.row << " - " << perfMove.moveCoords.col << std::endl;
		}
		else {
			std::cout << "no piece there!!" << std::endl;
		}
		return perfMove;

	}
}

const std::vector<Move> Engine::moveGenerator(Board& board)
{
	std::vector<Move> MoveList;
	for (auto pieceFromBoard : board) {
		if (pieceFromBoard->isWhite == board.isWhiteTurn)
		{
			pieceFromBoard->setLegalMoves(board);
			for (auto const move : pieceFromBoard->posMoves) {
				if (pieceFromBoard->getName() == "pawn" && move.row == (board.isWhiteTurn ? 7 : 0)) {
					MoveList.push_back(Move(pieceFromBoard->getCurrentField(), move, true, "queen"));
					MoveList.push_back(Move(pieceFromBoard->getCurrentField(), move, true, "rook"));
					MoveList.push_back(Move(pieceFromBoard->getCurrentField(), move, true, "bishop"));
					MoveList.push_back(Move(pieceFromBoard->getCurrentField(), move, true, "knight"));
				}
				else {
					MoveList.push_back(Move(pieceFromBoard->getCurrentField(), move, false, ""));
				}
			}
		}
	}
	return MoveList;
}

void Engine::createPromotionPiece(Board& board, Move move)
{
	if (move.PromotionPiece == "queen") {
		board[move.pieceCoords.row][move.pieceCoords.col] = std::make_shared<Queen>(move.pieceCoords.row, move.pieceCoords.col, board.isWhiteTurn);
	}
	else if (move.PromotionPiece == "knight") {
		board[move.pieceCoords.row][move.pieceCoords.col] = std::make_shared<Knight>(move.pieceCoords.row, move.pieceCoords.col, board.isWhiteTurn);
	}
	else if (move.PromotionPiece == "rook") {
		board[move.pieceCoords.row][move.pieceCoords.col] = std::make_shared<Rook>(move.pieceCoords.row, move.pieceCoords.col, board.isWhiteTurn);
	}
	else if (move.PromotionPiece == "bishop") {
		board[move.pieceCoords.row][move.pieceCoords.col] = std::make_shared<Bishop>(move.pieceCoords.row, move.pieceCoords.col, board.isWhiteTurn);
	}
	board[move.pieceCoords.row][move.pieceCoords.col]->gotMoved = true;
}