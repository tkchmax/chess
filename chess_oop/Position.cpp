#include "Position.h"
Position::Position()
{
	figures_ = vector<vector<shared_ptr<Figure>>>(2, vector<shared_ptr<Figure>>(7));
	figureFromCoord_ = vector<vector<int>>(2, vector<int>(64));

	figures_[WHITE][PAWN] = shared_ptr<Pawn>(new Pawn(WHITE));
	figures_[BLACK][PAWN] = shared_ptr<Pawn>(new Pawn(BLACK));

	figures_[WHITE][KNIGHT] = shared_ptr<Knight>(new Knight(WHITE));
	figures_[BLACK][KNIGHT] = shared_ptr<Knight>(new Knight(BLACK));

	figures_[WHITE][BISHOP] = shared_ptr<Bishop>(new Bishop(WHITE));
	figures_[BLACK][BISHOP] = shared_ptr<Bishop>(new Bishop(BLACK));

	figures_[WHITE][ROOK] = shared_ptr<Rook>(new Rook(WHITE));
	figures_[BLACK][ROOK] = shared_ptr<Rook>(new Rook(BLACK));

	figures_[WHITE][QUEEN] = shared_ptr<Queen>(new Queen(WHITE));
	figures_[BLACK][QUEEN] = shared_ptr<Queen>(new Queen(BLACK));

	figures_[WHITE][KING] = shared_ptr<King>(new King(WHITE));
	figures_[BLACK][KING] = shared_ptr<King>(new King(BLACK));

	U64 t_board = 0;
	for (int color = 0; color < 2; ++color)
	{
		for (int type = 1; type < 7; ++type)
		{
			t_board = figures_[color][type]->getBoard();
			while (t_board) {
				int square = BitScanForward(t_board);
				figureFromCoord_[color][square] = type;
				t_board &= t_board - 1;
			}
		}
	}

}

vector<vector<shared_ptr<Figure>>> Position::getFigures()
{
	return figures_;
}

U64 Position::getSideBoard(int color)
{
	U64 board = figures_[color][PAWN]->getBoard() | figures_[color][KNIGHT]->getBoard() | figures_[color][BISHOP]->getBoard() | figures_[color][ROOK]->getBoard()
		| figures_[color][QUEEN]->getBoard() | figures_[color][KING]->getBoard();
	return board;
}

U64 Position::getAllFiguresBoard()
{
	return getSideBoard(WHITE) | getSideBoard(BLACK);
}

MoveList Position::getFigureMoveList(int figure, int color)
{
	return figures_[color][figure]->getAvailibleMoves(*this);
}

U64 Position::getFigureBoard(int type, int color)
{
	return figures_[color][type]->getBoard();
}

int Position::getFigureOnSquare(int square)
{
	int wFigure = figureFromCoord_[WHITE][square];
	int bFigure = figureFromCoord_[BLACK][square];
	
	if (wFigure == 0)
		return bFigure;
	return wFigure;
}

int Position::getSideFiguresCount(int color)
{
	int count = 0;
	for (int type = PAWN; type <= KING; ++type)
		count += figures_[color][type]->getFigureCount();
	return count;
}

vector<vector<int>> Position::getFigureFromCoord()
{
	return figureFromCoord_;
}

bool Position::isMoveLegal(int move)
{
	Position testPosition(*this);

	int move_figure = READ_FIGURE(move);
	int move_from = READ_FROM(move);
	int move_to = READ_TO(move);
	int move_color = READ_COLOR(move);

	testPosition.figures_[move_color][move_figure]->moveFigure(move_from, move_to);

	if (testPosition.isKingAttacked(move_color))
		return false;
	return true;
}

U64 Position::getAtackRays(int color)
{
	int opColor = (color == WHITE ? BLACK : WHITE);
	U64 blockers = getSideBoard(color);
	U64 opposite = getSideBoard(opColor);

	vector<shared_ptr<Figure>> t_board = figures_[color];
	U64 rays = 0;

	for (int type = PAWN; type <= KING; ++type)
		rays |= figures_[color][type]->getAttackBoard(blockers, opposite);

	return rays;
}

void Position::setFigures(vector<vector<shared_ptr<Figure>>> figures)
{
	figures_ = figures;
}

void Position::setFigureFromCoord(vector<vector<int>> figureFromCoord)
{
	figureFromCoord_ = figureFromCoord;
}

bool Position::isKingAttacked(int color)
{
	int oppositeColor = (color == WHITE ? BLACK : WHITE);
	U64 attackers = getAtackRays(oppositeColor);
	if (attackers & figures_[color][KING]->getBoard())
		return true;
	return false;
}

