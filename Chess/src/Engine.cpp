#include "Engine.h"
#include "Game.h"
#include "UI.h"
#include <vector>
const void Engine::visualizeBoard(Board& board) {
	sf::RenderWindow windowEngine(sf::VideoMode(800, 800), "TestWindow");
	windowEngine.setFramerateLimit(30);
	Game game{};
	UI ui{ &windowEngine, game };
	ui.drawBoardtest(board);
}

int Engine::evalPosition(Board& board) {
	
}

int Engine::createTree(Board& board, int depth) {
	if (depth == 0) {
		return evalPosition(board);
	}
	else {
		// calculate all possible moves
		for (auto pieceFromBoard : board) {
			if (pieceFromBoard->isWhite == board.isWhiteTurn)
				pieceFromBoard->setLegalMoves(board);
		}

		//std::vector<Board> boardvec;

		for (auto piece : board) {
			if (piece->isWhite == board.isWhiteTurn) {
				for (auto& move : piece->posMoves) {
					//std::vector<evalMove> savebestVariant;
					piece->gotMoved = true;
					Board newBoard = board;
					//boardvec.push_back(newBoard);

					newBoard.move(newBoard[piece->getCurrentField().row][piece->getCurrentField().col], move.row, move.col);
					newBoard.isWhiteTurn = !newBoard.isWhiteTurn;
					//visualize
					visualizeBoard(newBoard);
					auto subtreevalue = createTree(newBoard, depth - 1);


					//compare all childrens value by calling return value of createTree for each child
					//
					// check black/white
					// if white:
						// return maxval of all children 
					// new val = min/max(createTree) 
					//savebestVariant.push_back()

				}
			}
		}
	}
	return 1;
}