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

	isKingMoved_ = vector<bool>(2);
	isRshRookMoved_ = vector<bool>(2);
	isLshRookMoved_ = vector<bool>(2);
}

class MvvLva
{
public:
	inline bool operator()(int a, int b)
	{
		int c1, c2;
		c1 = READ_CAPTURE(a) - READ_FIGURE(a);
		c2 = READ_CAPTURE(b) - READ_FIGURE(b);
		return c1 > c2;
	}
};

Position::Position(const Position& position)
{
	
}

vector<vector<shared_ptr<Figure>>> Position::getFigures() const
{
	return figures_;
}

U64 Position::getSideBoard(int color) const
{
	U64 board = figures_[color][PAWN]->getBoard() | figures_[color][KNIGHT]->getBoard() | figures_[color][BISHOP]->getBoard() | figures_[color][ROOK]->getBoard()
		| figures_[color][QUEEN]->getBoard() | figures_[color][KING]->getBoard();
	return board;
}

U64 Position::getAllFiguresBoard()
{
	return getSideBoard(WHITE) | getSideBoard(BLACK);
}

MoveList Position::getFigureMoveList(int figure, int color) const
{
	return figures_[color][figure]->getAvailibleMoves(*this);
}

MoveList Position::getMoves(int color, int movesType) const
{
	MoveList moves;
	for (int type = PAWN; type <= KING; ++type)
		moves += figures_[color][type]->getAvailibleMoves(*this, movesType);

	if(movesType != MOVE_TYPE_SILENT)
		sort(moves.capture.begin(), moves.capture.end(), MvvLva()); // MVV-LVA

	//add castling moves
	if (movesType != MOVE_TYPE_TAKE)
	{
		int from = (color == WHITE) ? 4 : 60;

		if (isCastlingPossible(color, MOVE_TYPE_0_0))
		{
			int to = (color == WHITE) ? 6 : 62;
			moves += CreateListItem(from, to, KING, 0, MOVE_TYPE_0_0, color);
		}
		if (isCastlingPossible(color, MOVE_TYPE_0_0_0))
		{
			int to = (color == WHITE) ? 2 : 58;
			moves += CreateListItem(from, to, KING, 0, MOVE_TYPE_0_0_0, color);
		}

	}
		
	return moves;
}


U64 Position::getFigureBoard(int type, int color) const
{
	return figures_[color][type]->getBoard();
}

int Position::getFigureOnSquare(int square, int color) const
{
	if (figureFromCoord_[BLACK][3] == KING)
		cout << "";
	return figureFromCoord_[color][square];
}

int Position::getSideFiguresCount(int color) const
{
	int count = 0;
	for (int type = PAWN; type <= KING; ++type)
		count += figures_[color][type]->getCount();
	return count;
}

int Position::getFigureCount(int figure, int color)
{
	return figures_[color][figure]->getCount();
}

void Position::empty()
{
	*this = Position();

	for (int color = 0; color < 2; ++color)
	{
		for (int type = PAWN; type <= KING; ++type)
		{
			figures_[color][type]->setBoard(0);
			figures_[color][type]->setCount(0);
		}
		for (int i = 0; i < 64; ++i)
			figureFromCoord_[color][i] = 0;
	}


}

vector<vector<int>> Position::getFigureFromCoord() const
{
	return figureFromCoord_;
}

bool Position::isMoveLegal(int move) const
{
	int move_figure = READ_FIGURE(move);
	int move_from = READ_FROM(move);
	int move_to = READ_TO(move);
	int move_color = READ_COLOR(move);

	figures_[move_color][move_figure]->moveFigure(move_from, move_to);

	if (isKingAttacked(move_color))
	{
		figures_[move_color][move_figure]->moveFigure(move_to, move_from);
		return false;
	}
	figures_[move_color][move_figure]->moveFigure(move_to, move_from);

	return true;
}

U64 Position::getAtackRays(int color) const
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

bool Position::isKingAttacked(int color) const
{
	int oppositeColor = (color == WHITE ? BLACK : WHITE);
	U64 attackers = getAtackRays(oppositeColor);
	if (attackers & figures_[color][KING]->getBoard())
		return true;
	return false;
}

bool Position::isCastlingPossible(int color, int castlingType) const
{
	if (castlingType == MOVE_TYPE_0_0_0)
	{
		U64 castlingBlockers = (color == WHITE ? 0xE : 0xE00000000000000);
		bool isBlockersBetweenKingAndRook = getSideBoard(color) & castlingBlockers;
		if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isLshRookMoved_[color])
			return false;
	}
	else if (castlingType == MOVE_TYPE_0_0)
	{
		U64 castlingBlockers = (color == WHITE ? 0x60 : 0x6000000000000000);
		bool isBlockersBetweenKingAndRook = getSideBoard(color) & castlingBlockers;
		if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isRshRookMoved_[color])
			return false;
	}
	return true;
}

