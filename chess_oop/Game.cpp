#include "Game.h"

int CountHighBits(U64 bitboard)
{
	int count = 0;
	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}
	return count;
}


Game::Game()
{
	firstKingMove = vector<int>(2);
	firstLshRookMove = vector<int>(2);
	firstRshRookMove = vector<int>(2);
}

void Game::makeMove(int move)
{

	int move_figure = READ_FIGURE(move);
	int move_to = READ_TO(move);
	int move_from = READ_FROM(move);
	int move_type = READ_MOVE_TYPE(move);
	int move_capture = READ_CAPTURE(move);
	int move_color = READ_COLOR(move);

	int oppositeColor = (move_color == WHITE ? BLACK : WHITE);

	if (move_figure == KING && !isKingMoved_[move_color])
	{
		firstKingMove[move_color] = move;
		isKingMoved_[move_color] = true;
	}
	else if (move_figure == ROOK)
	{
		int lshRookStartPosition = (move_color == WHITE) ? 0 : 56;
		int rshRookStartPosition = (move_color == WHITE) ? 7 : 63;
		if (!isLshRookMoved_[move_color] && move_from == lshRookStartPosition)
		{
			firstLshRookMove[move_color] = move;
			isLshRookMoved_[move_color] = true;
		}
		else if (!isRshRookMoved_[move_color] && move_from == rshRookStartPosition)
		{
			firstRshRookMove[move_color] = move;
			isRshRookMoved_[move_color] = true;
		}
	}

	//Set new figure position 
	figures_[move_color][move_figure]->moveFigure(move_from, move_to);
	figureFromCoord_[move_color][move_from] = 0;
	figureFromCoord_[move_color][move_to] = move_figure;

	//remove capture figure
	if (move_capture)
	{
		figures_[oppositeColor][move_capture]->removePiece(move_to);
		figureFromCoord_[oppositeColor][move_to] = 0;
	}

	switch (move_type)
	{
	case MOVE_TYPE_0_0:
		_makeShortCastling(move_color);
		break;
	case MOVE_TYPE_0_0_0:
		_makeLongCastling(move_color);
		break;
	case PAWN_TO_QUEEN:
		_makePawnTransform(QUEEN, move_to, move_color);
		break;
	case PAWN_TO_KNIGHT:
		_makePawnTransform(KNIGHT, move_to, move_color);
		break;
	case PAWN_TO_BISHOP:
		_makePawnTransform(BISHOP, move_to, move_color);
		break;
	case PAWN_TO_ROOK:
		_makePawnTransform(ROOK, move_to, move_color);
		break;
	}

}

void Game::undoMove(int move)
{
	int move_figure = READ_FIGURE(move);
	int move_to = READ_TO(move);
	int move_from = READ_FROM(move);
	int move_type = READ_MOVE_TYPE(move);
	int move_capture = READ_CAPTURE(move);
	int move_color = READ_COLOR(move);

	int oppositeColor = (move_color == WHITE ? BLACK : WHITE);

	if (move_figure == KING && move == firstKingMove[move_color])
	{
		isKingMoved_[move_color] = false;
	}
	else if (move_figure == ROOK)
	{
		if (move == firstLshRookMove[move_color])
		{
			isLshRookMoved_[move_color] = false;
		}
		else if (move == firstRshRookMove[move_color])
		{
			isRshRookMoved_[move_color] = false;
		}
	}


	figures_[move_color][move_figure]->moveFigure(move_to, move_from);
	figureFromCoord_[move_color][move_to] = 0;
	figureFromCoord_[move_color][move_from] = move_figure;

	//restore capture figure
	if (move_capture)
	{
		figures_[oppositeColor][move_capture]->setFigureOnSquare(move_to);
		figureFromCoord_[oppositeColor][move_to] = move_capture;
	}
	switch (move_type)
	{
	case MOVE_TYPE_0_0:
		_undoShortCastling(move_color);
		break;
	case MOVE_TYPE_0_0_0:
		_undoLongCastling(move_color);
		break;
	case PAWN_TO_QUEEN:
		_undoPawnTransform(move_from, QUEEN, move_to, move_color);
		break;
	case PAWN_TO_KNIGHT:
		_undoPawnTransform(move_from, KNIGHT, move_to, move_color);
		break;
	case PAWN_TO_BISHOP:
		_undoPawnTransform(move_from, BISHOP, move_to, move_color);
		break;
	case PAWN_TO_ROOK:
		_undoPawnTransform(move_from, ROOK, move_to, move_color);
		break;
	}

}

int Game::evaluate(int color)
{
	int materialEval = getMaterialEval(color);
	int strategyEval = getStrategyEval(color);
	int mobilityEval = getMobilityEval(color);

	return materialEval + strategyEval + (0.05 * mobilityEval);
}

int Game::getMaterialEval(int color)
{
	//int eval = 0;
	//int nWhiteFigures = 0;
	//int nBlackFigures = 0;

	int eval = 0;
	for (int type = PAWN; type <= KING; ++type)
	{
		eval += (figures_[WHITE][type]->getCount() * figures_[WHITE][type]->getCost())
			- (figures_[BLACK][type]->getCount() * figures_[BLACK][type]->getCost());

		/*	nWhiteFigures = figures_[WHITE][type]->getFigureCount();
			nBlackFigures = figures_[BLACK][type]->getFigureCount();

			switch (type)
			{
			case KING:
				eval += (nWhiteFigures * KING_VALUE) - (nBlackFigures * KING_VALUE);
				break;
			case QUEEN:
				eval += (nWhiteFigures * QUEEN_VALUE) - (nBlackFigures * QUEEN_VALUE);
				break;
			case ROOK:
				eval += (nWhiteFigures * ROOK_VALUE) - (nBlackFigures * ROOK_VALUE);
				break;
			case BISHOP:
				eval += (nWhiteFigures * BISHOP_VALUE) - (nBlackFigures * BISHOP_VALUE);
				break;
			case KNIGHT:
				eval += (nWhiteFigures * KNIGHT_VALUE) - (nBlackFigures * KNIGHT_VALUE);
				break;
			case PAWN:
				eval += (nWhiteFigures * PAWN_VALUE) - (nBlackFigures * PAWN_VALUE);
				break;
			}*/
	}

	if (color == WHITE) return eval;
	return -eval;
}

int Game::getStrategyEval(int color)
{
	vector<int> whitePiecesSquares;
	vector<int> blackPiecesSquares;
	int eval = 0;
	for (int type = PAWN; type <= KING; ++type)
	{
		whitePiecesSquares = figures_[WHITE][type]->getPiecesSquares();
		blackPiecesSquares = figures_[BLACK][type]->getPiecesSquares();

		for (int i = 0; i < whitePiecesSquares.size(); ++i)
			eval += figures_[WHITE][type]->getPriorityEvalOnSquare(whitePiecesSquares[i]);
		for (int j = 0; j < blackPiecesSquares.size(); ++j)
			eval -= figures_[BLACK][type]->getPriorityEvalOnSquare(blackPiecesSquares[j]);
	}

	if (color == WHITE) return eval;
	return -eval;
}

int Game::getMobilityEval(int color)
{
	return 0;
}

bool Game::isGameOver()
{
	if (figures_[WHITE][KING]->getBoard() == 0 || figures_[BLACK][KING]->getBoard() == 0)
		return true;
	return false;
}

void Game::setIsKingMoved(int color, bool isMoved)
{
	isKingMoved_[color] = isMoved;
}

void Game::setIsLshRookMoved(int color, bool isMoved)
{
	isLshRookMoved_[color] = isMoved;
}

void Game::setIsRshRookMoved(int color, bool isMoved)
{
	isRshRookMoved_[color] = isMoved;
}

void Game::setFEN(string fen)
{
	//for (int i = 0; i < 64; ++i)
	//{
	//	figureFromCoord_[WHITE][i] = 0;
	//	figureFromCoord_[BLACK][i] = 0;
	//}
	//for (int i = 1; i < 7; ++i)
	//{
	//	figures_[WHITE][i] = 0;
	//	figures_[BLACK][i] = 0;
	//}_

	this->empty();

	int board_cur = 72;
	int cur = -1;
	for (int i = 0; i < 8; ++i) {
		board_cur -= 16;
		cur++;
		while (fen[cur] != '/')
		{
			switch (fen[cur])
			{
			case 'K':
				figures_[WHITE][KING]->setFigureOnSquare(board_cur);
				//figures_[WHITE][KING] = 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = KING;
				board_cur++;
				break;
			case 'k':
				figures_[BLACK][KING]->setFigureOnSquare(board_cur);
				//figures_[BLACK][KING] = 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = KING;
				board_cur++;
				break;
			case 'Q':
				figures_[WHITE][QUEEN]->setFigureOnSquare(board_cur);
				//figures_[WHITE][QUEEN] |= 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = QUEEN;
				board_cur++;
				break;
			case 'q':
				figures_[BLACK][QUEEN]->setFigureOnSquare(board_cur);
				//figures_[BLACK][QUEEN] |= 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = QUEEN;
				board_cur++;
				break;
			case 'B':
				figures_[WHITE][BISHOP]->setFigureOnSquare(board_cur);
				//figures_[WHITE][BISHOP] |= 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = BISHOP;
				board_cur++;
				break;
			case 'b':
				figures_[BLACK][BISHOP]->setFigureOnSquare(board_cur);
				//figures_[BLACK][BISHOP] |= 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = BISHOP;
				board_cur++;
				break;
			case 'N':
				figures_[WHITE][KNIGHT]->setFigureOnSquare(board_cur);
				//figures_[WHITE][KNIGHT] |= 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = KNIGHT;
				board_cur++;
				break;
			case 'n':
				figures_[BLACK][KNIGHT]->setFigureOnSquare(board_cur);
				//figures_[BLACK][KNIGHT] |= 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = KNIGHT;
				board_cur++;
				break;
			case 'P':
				figures_[WHITE][PAWN]->setFigureOnSquare(board_cur);
				//figures_[WHITE][PAWN] |= 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = PAWN;
				board_cur++;
				break;
			case 'p':
				figures_[BLACK][PAWN]->setFigureOnSquare(board_cur);
				//figures_[BLACK][PAWN] |= 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = PAWN;
				board_cur++;
				break;
			case 'R':
				figures_[WHITE][ROOK]->setFigureOnSquare(board_cur);
				//figures_[WHITE][ROOK] |= 1ULL << board_cur;
				figureFromCoord_[WHITE][board_cur] = ROOK;
				board_cur++;
				break;
			case 'r':
				figures_[BLACK][ROOK]->setFigureOnSquare(board_cur);
				//figures_[BLACK][ROOK] |= 1ULL << board_cur;
				figureFromCoord_[BLACK][board_cur] = ROOK;
				board_cur++;
				break;
			default:
				board_cur += (fen[cur] - '0');
				break;
			}
			cur++;
		}
	}
}

void Game::_makeShortCastling(int color)
{
	isRshRookMoved_[color] = true;

	//set new rook position
	int move_from = (color == WHITE ? 7 : 63);
	int move_to = (color == WHITE ? 5 : 61);
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;

}

void Game::_makeLongCastling(int color)
{
	isLshRookMoved_[color] = true;

	//set new rook position
	int move_from = (color == WHITE ? 0 : 56);
	int move_to(color == WHITE ? 3 : 59);
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;

}

void Game::_makePawnTransform(int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->removePiece(transform_square);
	figures_[color][transform_in]->setFigureOnSquare(transform_square);

	figureFromCoord_[color][transform_square] = transform_in;
}

void Game::_undoShortCastling(int color)
{
	isRshRookMoved_[color] = false;

	//restore rook
	int move_from = (color == WHITE ? 7 : 63);
	int move_to = (color == WHITE ? 5 : 61);
	figures_[color][ROOK]->moveFigure(move_to, move_from);
	figureFromCoord_[color][move_from] = ROOK;
	figureFromCoord_[color][move_to] = 0;

}

void Game::_undoLongCastling(int color)
{
	isLshRookMoved_[color] = false;
	//restore rook
	int move_from = (color == WHITE ? 0 : 56);
	int move_to = (color == WHITE ? 3 : 59);
	bool b = figures_[color][ROOK]->moveFigure(move_to, move_from);
	if (!b)
		cout << "";
	figureFromCoord_[color][move_from] = ROOK;
	figureFromCoord_[color][move_to] = 0;
}

void Game::_undoPawnTransform(int move_from, int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->setFigureOnSquare(move_from);
	figures_[color][transform_in]->removePiece(transform_square);

	figureFromCoord_[color][transform_square] = 0;
}

