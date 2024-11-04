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
	int totalval = 0;
	for (auto piece : board) {
		if (piece->isWhite)
			totalval += piece->pieceval;
		else
			totalval -= piece->pieceval;
	}
	return totalval;
}

int Engine::createTree(Board& board, int depth, int& counter, sf::RenderWindow& wind) {
	static int originalDepth = -1;
	static int numpos = 0;	
	static int lastPrintedPercentage = -1;
	int flag = 0;
	if (originalDepth == -1) {
		originalDepth = depth;
		switch (originalDepth) {
		case 1: numpos = 20; break;
		case 2: numpos = 400; break;
		case 3: numpos = 8902; break;
		case 4: numpos = 197281; break;
		case 5: numpos = 4865609; break;
		case 6: numpos = 119060324; break;
		default: numpos = 0; break; // Handle invalid depths
		}
	}
	
	if (depth == 0) {
		counter++;
		int progressPercentage = (counter * 100) / numpos;
		// Check if the progress percentage is a multiple of 5 and hasn't been printed yet
		if (progressPercentage % 5 == 0 && progressPercentage != lastPrintedPercentage) {
			std::cout << "Progress: " << progressPercentage << "%" << std::endl;
			lastPrintedPercentage = progressPercentage; // Update last printed percentage
		}
		//std::cout << counter << std::endl;
		return evalPosition(board);
	}
	else {
		// calculate all possible moves
		for (auto pieceFromBoard : board) {
			if (pieceFromBoard->isWhite == board.isWhiteTurn)
				pieceFromBoard->setLegalMoves(board);
		}

		//std::vector<Board> boardvec;
		int minval = -10000;
		//Board minboard;
		for (auto piece : board) {
			if (piece->isWhite == board.isWhiteTurn) {
				for (auto& move : piece->posMoves) {
					Board newBoard = board;
					newBoard.move(newBoard[piece->getCurrentField().row][piece->getCurrentField().col], move.row, move.col);
					newBoard.isWhiteTurn = !newBoard.isWhiteTurn;
					//visualize
					//visualizeBoard(wind, newBoard);

					auto subtreevalue = createTree(newBoard, depth - 1, counter, wind);
					if (subtreevalue > minval) {
						//minboard = newBoard;
						minval = subtreevalue;
					}
					flag++;
				}
			}
		}
		//sf::RenderWindow windowEngine;
		//visualizeBoard(windowEngine,minboard);
		if (flag == 0) {
			std::cout << "care!"<<std::endl;
		}
		return counter;
	}
}

int Engine::createNoCopyTree(Board& board, int depth, int& counter, sf::RenderWindow& wind) {
	if (depth == 0) {
		counter++;
		std::cout << counter << std::endl;
		return evalPosition(board);
	}
	else {

		// calculate all possible moves
		for (auto pieceFromBoard : board) {
			if (pieceFromBoard->isWhite == board.isWhiteTurn)
				pieceFromBoard->setLegalMoves(board);
		}

		//std::vector<Board> boardvec;
		int minval = -10000;
		//Board minboard;
		for (const auto piece : board) {
			if (piece->isWhite == board.isWhiteTurn) {
				for (const auto& move : piece->posMoves) {
					auto saveEnemyPiece = board[move.row][move.col];
					auto savemoveField = move;
					auto pieceCopy = piece->clone();

					//move Piece
					board.move(board[piece->getCurrentField().row][piece->getCurrentField().col], move.row, move.col);
					board.isWhiteTurn = !board.isWhiteTurn;
					//visualize
					visualizeBoard(wind, board);
					auto subtreevalue = createNoCopyTree(board, depth - 1, counter, wind);
					//undo move
					board[pieceCopy->getCurrentField().row][pieceCopy->getCurrentField().col] = piece;
					board[piece->getCurrentField().row][piece->getCurrentField().col] = (saveEnemyPiece ? saveEnemyPiece : nullptr);
					piece->gotMoved = pieceCopy->gotMoved;
					piece->posMoves = pieceCopy->posMoves;
					piece->currentField = pieceCopy->currentField;
					board.isWhiteTurn = !board.isWhiteTurn;

					visualizeBoard(wind, board);

					if (subtreevalue > minval) {
						//minboard = newBoard;
						minval = subtreevalue;
					}
				}
			}
		}
		//sf::RenderWindow windowEngine;
		//visualizeBoard(windowEngine,minboard);
		return minval;
	}
}