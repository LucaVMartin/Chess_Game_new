#include "Engine.h"
#include "UI.h"
#include <vector>
#include <iostream>
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
			return (board.isWhiteTurn ? INT_MAX : INT_MIN); //return high/low value if checkmate
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

int Engine::createTree(Board& board, int depth, Move& bestMove) {
	int flagGameEnd = 0; //flag to check for game ends before leaf node
	static int originalDepth = -1;
	static int numpos = 0;
	static int lastPrintedPercentage = -1;
	if (this->testing && originalDepth == -1) {
		//static Move bestMove;
		originalDepth = depth;
		numpos = giveNumPos(originalDepth);
	}
	if (depth == 0) {
		if (this->testing)
		{
			positionctr++;
			//visualizeBoard(wind, board);
			//add return 0 for stalemate
			int progressPercentage = (positionctr * 100) / numpos;
			// Check if the progress percentage is a multiple of 5 and hasn't been printed yet
			if (progressPercentage % 5 == 0 && progressPercentage != lastPrintedPercentage) {
				std::cout << "Progress: " << progressPercentage << "%" << std::endl;
				lastPrintedPercentage = progressPercentage; // Update last printed percentage
			}
		}

		//std::cout << positionctr << std::endl;
		auto eval = this->evalPosition(board);
		return eval; //return board value 
	}
	else {
		// calculate all possible moves
		for (auto pieceFromBoard : board) {
			if (pieceFromBoard->isWhite == board.isWhiteTurn)
				pieceFromBoard->setLegalMoves(board);
		}

		int max = INT_MIN;
		//Board minboard;
		for (auto piece : board) {
			if (piece->isWhite == board.isWhiteTurn) {
				for (auto& move : piece->posMoves) {
					Board newBoard = board;
					if (depth == 1 && this->testing) {
						this->countCaptures(board, move);//count captures for testing (only at leaf nodes)
					}
					newBoard.move(newBoard[piece->getCurrentField().row][piece->getCurrentField().col], move.row, move.col);
					newBoard.isWhiteTurn = !newBoard.isWhiteTurn;
					//visualize					
					//visualizeBoard(this->wind, newBoard);
					auto eval = -createTree(newBoard, depth - 1, bestMove); //call recursion

					if (eval >= max) {
						max = eval;
						bestMove.moveCoords = move;
						bestMove.pieceCoords.row = piece->getCurrentField().row;
						bestMove.pieceCoords.col = piece->getCurrentField().col;
					}
					flagGameEnd++; // to check if no move was done;
				}
			}
		}
		//sf::RenderWindow windowEngine;
		//visualizeBoard(windowEngine,minboard);
		if (flagGameEnd == 0) {
			board.isWhiteTurn = board.isWhiteTurn;
			auto check = updateCheckStatus(board);
			//visualizeBoard(wind, board);
			if (check) {
				std::cout << "checkmate before leaf node" << std::endl;
				return (board.isWhiteTurn ? INT_MAX : INT_MIN);
			}
			else { std::cout << "stalemate before leaf node" << std::endl; return 0; }
		}
		return max;
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
