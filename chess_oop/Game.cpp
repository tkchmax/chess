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
	isKingMoved_ = vector<int>(2);
	isRshRookMoved_ = vector<int>(2);
	isLshRookMoved_ = vector<int>(2);
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

	//Set figure on previous position 
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
		break;
	case PAWN_TO_QUEEN:
		_undoPawnTransform(QUEEN, move_to, move_color);
		break;
	case PAWN_TO_KNIGHT:
		_undoPawnTransform(KNIGHT, move_to, move_color);
		break;
	case PAWN_TO_BISHOP:
		_undoPawnTransform(BISHOP, move_to, move_color);
		break;
	case PAWN_TO_ROOK:
		_undoPawnTransform(ROOK, move_to, move_color);
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
		eval += (figures_[WHITE][type]->getFigureCount() * figures_[WHITE][type]->getCost())
			- (figures_[BLACK][type]->getFigureCount() * figures_[BLACK][type]->getCost());

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

bool Game::_makeShortCastling(int color)
{
	U64 mask = (color == WHITE ? 0x6 : 0x600000000000000);
	bool isBlockersBetweenKingAndRook = getSideBoard(color) & mask;
	if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isRshRookMoved_[color])
		return false;

	isKingMoved_[color] = true;
	isRshRookMoved_[color] = true;

	int move_from = (color == WHITE ? 7 : 63);
	int move_to = (color == WHITE ? 5 : 61);
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;

	return true;
}

bool Game::_makeLongCastling(int color)
{
	U64 mask = (color == WHITE ? 0x70 : 0x7000000000000000);
	bool isBlockersBetweenKingAndRook = getSideBoard(color) & mask;
	if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isLshRookMoved_[color])
		return false;

	isKingMoved_[color] = true;
	isLshRookMoved_[color] = true;

	int move_from = (color == WHITE ? 0 : 56);
	int move_to(color == WHITE ? 3 : 59);
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;
	
	return true;
}

void Game::_makePawnTransform(int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->removePiece(transform_square);
	figures_[color][transform_in]->setFigureOnSquare(transform_square);

	figureFromCoord_[color][transform_square] = transform_in;
}

bool Game::_undoShortCastling(int color)
{
	U64 kingBoard = figures_[color][KING]->getBoard();
	bool isKingCastled = (color == WHITE ? kingBoard & (1ULL << 6) : kingBoard & (1ULL << 62));
	bool isRookCastled = (color == WHITE ? figures_[color][ROOK]->getBoard() & (1ULL << 5) : figures_[color][ROOK]->getBoard() & (1ULL << 61));
	if (isKingCastled && isRookCastled)
	{
		isKingMoved_[color] = false;
		isRshRookMoved_[color] = false;

		//restore rook
		int move_from = (color == WHITE ? 7 : 63);
		int move_to = (color == WHITE ? 5 : 61);
		figures_[color][ROOK]->moveFigure(move_to, move_from);
		figureFromCoord_[color][move_from] = ROOK;
		figureFromCoord_[color][move_to] = 0;

		return true;
	}
	return false;
	
}

bool Game::_undoLongCastling(int color)
{
	U64 kingBoard = figures_[color][KING]->getBoard();
	U64 rookBoard = figures_[color][ROOK]->getBoard();

	bool isKingCastled = (color == WHITE ? kingBoard & (1ULL << 2) : kingBoard & (1ULL << 58));
	bool isRookCastled = (color == WHITE ? rookBoard & (1ULL << 3) : rookBoard & (1ULL << 59));

	if (isKingCastled && isRookCastled)
	{
		isKingMoved_[color] = false;
		isLshRookMoved_[color] = false;

		//restore rook
		int move_from = (color == WHITE ? 0 : 56);
		int move_to = (color == WHITE ? 3 : 59);
		figures_[color][ROOK]->moveFigure(move_to, move_from);
		figureFromCoord_[color][move_from] = ROOK;
		figureFromCoord_[color][move_to] = 0;

		return true;
	}

	return false;
}

void Game::_undoPawnTransform(int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->setFigureOnSquare(transform_square - 8);
	figures_[color][transform_in]->removePiece(transform_square);

	figureFromCoord_[color][transform_square] = 0;
}
