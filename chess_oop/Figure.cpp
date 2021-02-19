#include "Figure.h"

vector<vector<U64>> rays(8, vector<U64>(64));

U64 _aOut(U64 bb)
{
	return (bb << 1) & NOT_A_FILE;
}
U64 _hOut(U64 bb)
{
	return(bb >> 1)& NOT_H_FILE;
}
void initializeRays()
{

	U64 north = 0x0101010101010100;
	U64 west = 9151314442816847872;
	U64 south = 0x0080808080808080;
	U64 east = 254;
	U64 north_east = 0x8040201008040200;
	U64 north_west = 72624976668147712;
	U64 south_west = 18049651735527937;
	U64 south_east = 567382630219904;

	//North
	for (int i = 0; i < 64; ++i, north <<= 1)
	{
		rays[NORTH][i] = north;
	}

	//West
	for (int x = 0; x < 8; ++x)
	{
		U64 w = west;
		west = _hOut(west);
		for (int y = 63; y >= 0; y -= 8, w >>= 8)
		{
			rays[WEST][y - x] = w;
		}
	}

	//South
	for (int i = 63; i >= 0; --i, south >>= 1)
	{
		rays[SOUTH][i] = south;
	}

	//East
	for (int x = 0; x < 8; ++x)
	{
		U64 e = east;
		east = _aOut(east);
		for (int y = 0; y < 64; y += 8, e <<= 8)
		{
			rays[EAST][x + y] = e;
		}
	}

	//North-East
	for (int y = 0; y < 8; ++y)
	{
		U64 ne = north_east;
		north_east = _aOut(north_east);
		for (int x = 0; x < 64; x += 8, ne <<= 8)
		{
			rays[NORTH_EAST][x + y] = ne;
		}
	}

	//North-West
	for (int x = 7; x >= 0; --x)
	{
		U64 nw = north_west;
		north_west = _hOut(north_west);
		for (int y = 0; y < 64; y += 8, nw <<= 8)
		{
			rays[NORTH_WEST][y + x] = nw;
		}
	}

	//South-West
	for (int x = 0; x < 8; x++)
	{
		U64 sw = south_west;
		south_west = _hOut(south_west);
		for (int y = 63; y >= 0; y -= 8, sw >>= 8)
		{
			rays[SOUTH_WEST][y - x] = sw;
		}
	}

	//South-East
	for (int x = 7; x >= 0; --x)
	{
		U64 se = south_east;
		south_east = _aOut(south_east);
		for (int y = 63; y >= 8; y -= 8, se >>= 8)
		{
			rays[SOUTH_EAST][y - x] = se;
		}
	}
}

U64 Figure::getAttackBoard(U64 blockers, U64 opposite)
{
	U64 attacks = 0;
	U64 t_board = board_;

	int square;
	while (t_board)
	{
		square = BitScanForward(t_board);
		attacks |= getMoveBoards(square, blockers, opposite).silents;

		t_board &= t_board - 1;
	}
	return attacks;
}

void Figure::setBoard(U64 board)
{
	board_ = board;
}

void Figure::setColor(int color)
{
	color_ = color;
}

U64 Figure::getBlockerPieces(Position& position)
{
	U64 blockers = (color_ == WHITE ? position.getFigureBoard(WHITE) : position.getFigureBoard(BLACK));
	return blockers;
}

U64 Figure::getOppositePieces(Position& position)
{
	U64 opposite = (color_ == WHITE ? position.getFigureBoard(BLACK) : position.getFigureBoard(WHITE));
	return opposite;
}

U64 Figure::getBoard()
{
	return board_;
}

U64 Figure::getColor()
{
	return color_;
}


//U64 Figure::getAtackRays(int color)
//{
	//int opColor = (color == WHITE ? BLACK : WHITE);
	//U64 blockers = figures[color][QUEEN] | figures[color][ROOK] | figures[color][BISHOP] | figures[color][KNIGHT] | figures[color][PAWN] | figures[color][KING];
	//U64 attackedFigures = figures[opColor][QUEEN] | figures[opColor][ROOK] | figures[opColor][BISHOP] | figures[opColor][KNIGHT] | figures[opColor][PAWN] | figures[opColor][KING];

	//vector<U64> bb = figures[color];
	//U64 rays = 0;

	//rays |= GetPawnCaptureMoves(bb[PAWN], color);

	//for (int fiure_type = 1; figure_type < 7; ++figure_type)
	//{
	//	while (bb[figure_type]) {
	//		int figure = BitScanForward(bb[figure_type]);
	//		switch (figure_type)
	//		{
	//		case KING:
	//			rays |= GetKingMoves(figure, blockers, attackedFigures, MOVE_TYPE_SILENT);
	//			break;
	//		case QUEEN:
	//			rays |= GetQueenMoves(figure, blockers, attackedFigures, MOVE_TYPE_SILENT);
	//			break;
	//		case ROOK:
	//			rays |= GetRookMoves(figure, blockers, attackedFigures, MOVE_TYPE_SILENT);
	//			break;
	//		case BISHOP:
	//			rays |= GetBishopMoves(figure, blockers, attackedFigures, MOVE_TYPE_SILENT);
	//			break;
	//		case KNIGHT:
	//			rays |= GetKnightMoves(figure, blockers, attackedFigures, MOVE_TYPE_SILENT);
	//			break;
	//		}
	//		bb[figure_type] &= bb[figure_type] - 1;
	//	}
	//}
	//return rays;
//}



Pawn::Pawn(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEPAWN_STARTPOSITION : BLACKPAWN_STARTPOSITION);
}

RawMoves Pawn::getMoveBoards(int square,  U64 blockers, U64 opposite)
{
	RawMoves moves;

	U64 allPieces = blockers | opposite;

	U64 figure_bit = 1ULL << square;

	U64 oneSquareMove = 0;
	U64 twoSquareMove = 0;
	U64 leftAttack = 0;
	U64 rightAttack = 0;
	
	if (color_ == WHITE)
	{

		oneSquareMove = figure_bit << 8;
		twoSquareMove = figure_bit << 16;
		leftAttack = (figure_bit << 9) & NOT_A_FILE;
		rightAttack = (figure_bit << 7) & NOT_H_FILE;

	}
	else {
		oneSquareMove = figure_bit >> 8;
		twoSquareMove = figure_bit >> 16;
		leftAttack = (figure_bit >> 7)& NOT_A_FILE;
		rightAttack = (figure_bit >> 9)& NOT_H_FILE;
	}

	bool isBlockerInFront = (oneSquareMove)&allPieces;
	bool isPawnOnStartingRank = figure_bit & WHITEPAWN_STARTPOSITION;
	if (!isBlockerInFront && isPawnOnStartingRank)
		moves.silents |= twoSquareMove & ~allPieces;
	moves.silents |= oneSquareMove & ~allPieces;

	if (leftAttack & opposite) moves.takes |= leftAttack;
	if (rightAttack & opposite) moves.takes |= rightAttack;

	return moves;
}

MoveList Pawn::getAvailibleMoves(Position& position)
{
	MoveList list;

	U64 pawns = board_;

	U64 blockers = getBlockerPieces(position);
	U64 opposite = getOppositePieces(position);
	vector<vector<int>> figureFromCoord = position.getFigureFromCoord();

	RawMoves moves;
	RawMoves t_moves;

	int square;
	while (pawns)
	{
		square = BitScanForward(pawns);
		t_moves = getMoveBoards(square, blockers, opposite);
		list.add(figureFromCoord, t_moves, square, color_, PAWN);

		pawns &= pawns - 1;
	}
	return list;
}

U64 Pawn::getAttackBoard(U64 blockers, U64 opposite)
{
	U64 attacks = Figure::getAttackBoard(blockers, opposite);
	U64 delTwoSquareMoves_mask = ~(color_ == WHITE ? WHITEPAWN_STARTPOSITION << 16 : BLACKPAWN_STARTPOSITION >> 16);

	return attacks & delTwoSquareMoves_mask;
}



Knight::Knight(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKNIGHT_STARTPOSITION : BLACKKNIGHT_STARTPOSITION);
}

RawMoves Knight::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	RawMoves moves;
	U64 board = 0;
	board |= (1ULL << square + 10) & (NOT_B_FILE & NOT_A_FILE);
	board |= (1ULL << square - 6) & (NOT_B_FILE & NOT_A_FILE);
	board |= (1ULL << square - 15) & NOT_A_FILE;
	board |= (1ULL << square - 17) & NOT_H_FILE;
	board |= (1ULL << square - 10) & (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << square + 6) & (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << square + 15) & NOT_H_FILE;
	board |= (1ULL << square + 17) & NOT_A_FILE;

	moves.silents = board & ~opposite & ~blockers;
	moves.takes = board & opposite;

	return moves;
}

MoveList Knight::getAvailibleMoves(Position& position)
{
	MoveList list;
	RawMoves rawMoves;

	U64 knights = board_;
	U64 blockers = getBlockerPieces(position);
	U64 opposite = getOppositePieces(position);
	vector<vector<int>> figureFromCoord = position.getFigureFromCoord();

	U64 moves;
	int square;

	while (knights)
	{
		square = BitScanForward(knights);
		rawMoves = getMoveBoards(square, blockers, opposite);

		list.add(figureFromCoord, rawMoves, square, color_, KNIGHT);

		knights &= knights - 1;
	}
	return list;
}




Bishop::Bishop(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEBISHOP_STARTPOSITION : BLACKBISHOP_STARTPOSITION);
}

RawMoves Bishop::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	return RawMoves();
}

MoveList Bishop::getAvailibleMoves(Position& game)
{
	return MoveList();
}




Rook::Rook(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEROOK_STARTPOSITION : BLACKROOK_STARTPOSITION);
}

RawMoves Rook::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	return RawMoves();
}

MoveList Rook::getAvailibleMoves(Position& game)
{
	return MoveList();
}



Queen::Queen(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEQUEEN_STARTPOSITION : BLACKQUEEN_STARTPOSITION);
}

RawMoves Queen::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	return RawMoves();
}

MoveList Queen::getAvailibleMoves(Position& game)
{
	return MoveList();
}




King::King(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKING_STARTPOSITION : BLACKKING_STARTPOSITION);
}

RawMoves King::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	return RawMoves();
}

MoveList King::getAvailibleMoves(Position& game)
{
	return MoveList();
}
