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

	U64 t_board=0;
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

U64 Position::getFigureBoard(int color)
{
	U64 board = figures_[color][PAWN]->getBoard() | figures_[color][KNIGHT]->getBoard() | figures_[color][BISHOP]->getBoard() | figures_[color][ROOK]->getBoard()
		| figures_[color][QUEEN]->getBoard() | figures_[color][KING]->getBoard();
	return board;
}

U64 Position::getAllFiguresBoard()
{
	return getFigureBoard(WHITE) | getFigureBoard(BLACK);
}

MoveList Position::getFigureMoveList(int figure, int color)
{
	return figures_[color][figure]->getAvailibleMoves(*this);
}

vector<vector<int>> Position::getFigureFromCoord()
{
	return figureFromCoord_;
}

U64 Position::getAtackRays(int color)
{
	int opColor = (color == WHITE ? BLACK : WHITE);
	U64 blockers = getFigureBoard(color);
	U64 opposite = getFigureBoard(opColor);

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

