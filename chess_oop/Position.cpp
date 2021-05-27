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

void Position::setFEN(string fen)
{
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


string Position::getFigureIdOnSquare(int square)
{
	int whiteFigure = figureFromCoord_[WHITE][square];
	int blackFigure = figureFromCoord_[BLACK][square];
	if (whiteFigure != 0 && blackFigure != 0)
		cout << "";

	string figure;
	if (whiteFigure)
	{
		switch (whiteFigure)
		{
		case PAWN:
			figure = "P";
			break;
		case KNIGHT:
			figure = "N";
			break;
		case BISHOP:
			figure = "B";
			break;
		case ROOK:
			figure = "R";
			break;
		case QUEEN:
			figure = "Q";
			break;
		case KING:
			figure = "K";
			break;
		}
	}
	else if (blackFigure)
	{
		switch (blackFigure)
		{
		case PAWN:
			figure = "p";
			break;
		case KNIGHT:
			figure = "n";
			break;
		case BISHOP:
			figure = "b";
			break;
		case ROOK:
			figure = "r";
			break;
		case QUEEN:
			figure = "q";
			break;
		case KING:
			figure = "k";
			break;
		}
	}
	else
	{
		figure = "0";
	}

	return figure;
}
string Position::getFEN()
{
	string fen;
	int noFigureCount = 0;

	string cur;

	vector<vector<string>> lines(8, vector<string>(8));

	for (int i = 0; i < 64; ++i)
	{
		lines[i / 8][i % 8] = getFigureIdOnSquare(i);
	}

	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			cur = lines[i][j];
			if (cur != "0")
			{
				if (noFigureCount)
				{
					ostringstream ss;
					ss << noFigureCount;
					fen += ss.str();
					noFigureCount = 0;
				}
				fen += cur;
			}
			else
				noFigureCount++;
		}

		if (noFigureCount)
		{
			ostringstream ss;
			ss << noFigureCount;
			fen += ss.str();
			noFigureCount = 0;
		}
		fen += "/";
		cout << endl;
	}


	return fen;
}

Position::Position(const Position& position)
{
	vector<vector<shared_ptr<Figure>>> copiedFigures(2, vector<shared_ptr<Figure>>(7));
	copiedFigures[WHITE][PAWN] = shared_ptr<Pawn>(new Pawn(WHITE));
	copiedFigures[BLACK][PAWN] = shared_ptr<Pawn>(new Pawn(BLACK));

	copiedFigures[WHITE][KNIGHT] = shared_ptr<Knight>(new Knight(WHITE));
	copiedFigures[BLACK][KNIGHT] = shared_ptr<Knight>(new Knight(BLACK));

	copiedFigures[WHITE][BISHOP] = shared_ptr<Bishop>(new Bishop(WHITE));
	copiedFigures[BLACK][BISHOP] = shared_ptr<Bishop>(new Bishop(BLACK));

	copiedFigures[WHITE][ROOK] = shared_ptr<Rook>(new Rook(WHITE));
	copiedFigures[BLACK][ROOK] = shared_ptr<Rook>(new Rook(BLACK));

	copiedFigures[WHITE][QUEEN] = shared_ptr<Queen>(new Queen(WHITE));
	copiedFigures[BLACK][QUEEN] = shared_ptr<Queen>(new Queen(BLACK));

	copiedFigures[WHITE][KING] = shared_ptr<King>(new King(WHITE));
	copiedFigures[BLACK][KING] = shared_ptr<King>(new King(BLACK));


	for (int color = 0; color < 2; ++color)
		for (int figure = PAWN; figure <= KING; ++figure)
		{
			//copiedFigures[color][figure] = shared_ptr<Pawn>(new Pawn(color));
			copiedFigures[color][figure]->clone(position.figures_[color][figure]);
		}

	this->setFigures(copiedFigures);
	this->figureFromCoord_ = position.figureFromCoord_;
	this->isKingMoved_ = position.isKingMoved_;
	this->isLshRookMoved_ = position.isLshRookMoved_;
	this->isRshRookMoved_ = position.isRshRookMoved_;
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

	if (movesType != MOVE_TYPE_SILENT)
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

shared_ptr<Figure> Position::getFigure(int figure, int color)
{
	return figures_[color][figure];
}

int Position::getFigureOnSquare(int square, int color) const
{
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
	//*this = Position();

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
	int move_type = READ_MOVE_TYPE(move);
	int move_capture = READ_CAPTURE(move);
	int oppositeColor = (move_color == WHITE) ? BLACK : WHITE;

	if (move_type == MOVE_TYPE_TAKE && move_capture == 0)
		return false;

	Position tempPosition(*this);

	tempPosition.figures_[move_color][move_figure]->moveFigure(move_from, move_to);
	tempPosition.figureFromCoord_[move_color][move_from] = NO_FIGURE;
	tempPosition.figureFromCoord_[move_color][move_to] = move_figure;

	if (move_type == MOVE_TYPE_TAKE)
	{
		int oppositeColor = (move_color == WHITE) ? BLACK : WHITE;
		tempPosition.figures_[oppositeColor][move_capture]->removePiece(move_to);
		tempPosition.figureFromCoord_[oppositeColor][move_to] = NO_FIGURE;
	}

	if (tempPosition.isKingAttacked(move_color))
	{
		return false;
	}


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
	//ShowBits(figures_[color][KING]->getBoard());
	//ShowBits(attackers);
	//cout << "---\n\n";
	if (attackers & figures_[color][KING]->getBoard())
		return true;
	return false;
}

//bool Position::isKingAttacked(const U64& kingBoard, U64& oppositePieces, U64& blockers)
//{
//	
//}

bool Position::isCastlingPossible(int color, int castlingType) const
{
	int oppositeColor = (color == WHITE) ? BLACK : WHITE;
	if (castlingType == MOVE_TYPE_0_0_0)
	{
		U64 castlingBlockers = (color == WHITE ? 0xE : 0xE00000000000000);
		bool isBlockersBetweenKingAndRook = getSideBoard(color) & castlingBlockers;
		bool isCastlingUnderAttack = getAtackRays(oppositeColor) & castlingBlockers;
		if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isLshRookMoved_[color] || isCastlingUnderAttack)
			return false;
	}
	else if (castlingType == MOVE_TYPE_0_0)
	{
		U64 castlingBlockers = (color == WHITE ? 0x60 : 0x6000000000000000);
		bool isBlockersBetweenKingAndRook = getSideBoard(color) & castlingBlockers;
		bool isCastlingUnderAttack = getAtackRays(oppositeColor) & castlingBlockers;
		if (isBlockersBetweenKingAndRook || isKingMoved_[color] || isRshRookMoved_[color] || isCastlingUnderAttack)
			return false;
	}
	return true;
}

