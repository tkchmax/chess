#include "Game.h"

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

	switch (move_type)
	{
	case MOVE_TYPE_TAKE:
		//remove capture figure
		figures_[oppositeColor][move_capture]->removePiece(move_to);
		figureFromCoord_[oppositeColor][move_to] = 0;
		break;
	case MOVE_TYPE_0_0:
		break;
	case MOVE_TYPE_0_0_0:
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


bool Game::_makeShortCastling(int color)
{
	return false;
}

bool Game::_makeLongCastling(int color)
{
	return false;
}

void Game::_makePawnTransform(int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->removePiece(transform_square);
	figures_[color][transform_in]->setFigureOnSquare(transform_square);

	figureFromCoord_[color][transform_square] = transform_in;
}
