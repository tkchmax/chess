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
void _initializeRays()
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


MoveList Figure::getAvailibleMoves(Position& position)
{
	MoveList list;

	U64 board = board_;

	U64 blockers = getBlockerPieces(position);
	U64 opposite = getOppositePieces(position);

	RawMoves moves;
	RawMoves t_moves;

	int from;
	int to;
	while (board)
	{
		from = BitScanForward(board);
		t_moves = getMoveBoards(from, blockers, opposite);

		U64 silentMoves = t_moves.silents;
		U64 captureMoves = t_moves.takes;

		int move;

		while (silentMoves)
		{
			to = BitScanForward(silentMoves);

			move = CreateListItem(from, to, PAWN, 0, MOVE_TYPE_SILENT, color_); 
			if (position.isMoveLegal(move))
				list += move;

			silentMoves &= silentMoves - 1;
		}
		int capture;
		while (captureMoves)
		{
			to = BitScanForward(captureMoves);
			capture = position.getFigureOnSquare(to);

			move = CreateListItem(from, to, PAWN, capture, MOVE_TYPE_TAKE, color_);
			if (position.isMoveLegal(move))
				list += move;

			captureMoves &= captureMoves - 1;
		}

		board &= board - 1;
	}
	return list;
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
	U64 blockers = (color_ == WHITE ? position.getSideBoard(WHITE) : position.getSideBoard(BLACK));
	return blockers;
}

U64 Figure::getOppositePieces(Position& position)
{
	U64 opposite = (color_ == WHITE ? position.getSideBoard(BLACK) : position.getSideBoard(WHITE));
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

bool Figure::moveFigure(int old_coordinates, int new_coordinates)
{
	bool isFigureExist = (1ULL << old_coordinates) & board_;
	if (!isFigureExist)
		return false;

	U64 newBoard = ~(1ULL << old_coordinates) & board_ | (1ULL << new_coordinates);
	board_ = newBoard;
	return true;
}

bool Figure::removePiece(int square)
{
	bool isFigureOnSquare = (1ULL << square) & board_;
	if (isFigureOnSquare)
	{
		board_ &= ~(1ULL << square);
		return true;
	}
	return false;

}

bool Figure::setFigureOnSquare(int square)
{
	bool isFigureAlreadyOnSquare = (1ULL << square) & board_;
	if (isFigureAlreadyOnSquare)
		return false;
	board_ |= (1ULL << square);
	return true;

}


Pawn::Pawn(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEPAWN_STARTPOSITION : BLACKPAWN_STARTPOSITION);
}

RawMoves Pawn::getMoveBoards(int square, U64 blockers, U64 opposite)
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

	const U64 TRANSFORM_RANK = (color_ == WHITE ? BLACKPAWN_STARTPOSITION >> 8 : WHITEPAWN_STARTPOSITION << 8);

	int from;
	int to;
	int move;
	U64 silentMoves=0, captureMoves=0;
	bool isTransformation;
	while (pawns)
	{
		from = BitScanForward(pawns);
		t_moves = getMoveBoards(from, blockers, opposite);

		 silentMoves = t_moves.silents;
		 captureMoves = t_moves.takes;
		 isTransformation;

		 move = 0;
		while (silentMoves)
		{
			to = BitScanForward(silentMoves);
			isTransformation = (1ULL << to) & TRANSFORM_RANK;
			if (isTransformation)
			{
				for (int i = KNIGHT; i <= QUEEN; ++i)
				{
					move = CreateListItem(from, to, PAWN, 0, PAWN_TO_KNIGHT + i - KNIGHT, color_);
					if (position.isMoveLegal(move))
						list += move;
				}
			}
			else {
				move = CreateListItem(from, to, PAWN, 0, MOVE_TYPE_SILENT, color_);
				if (position.isMoveLegal(move))
					list += move;
			}

			silentMoves &= silentMoves - 1;
		}

		int capture;
		while (captureMoves)
		{
			to = BitScanForward(captureMoves);
			capture = position.getFigureOnSquare(to);
			isTransformation = (1ULL << to) & TRANSFORM_RANK;
			if (isTransformation)
			{
				for (int i = KNIGHT; i <= QUEEN; ++i)
				{
					move = CreateListItem(from, to, PAWN, capture, PAWN_TO_KNIGHT + i - KNIGHT, color_);
					if (position.isMoveLegal(move))
						list += move;
				}
			}
			else
			{
				move = CreateListItem(from, to, PAWN, capture, MOVE_TYPE_TAKE, color_);
				if (position.isMoveLegal(move))
					list += move;
			}

			captureMoves &= captureMoves - 1;

		}

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

bool Pawn::_isPawnTransformMove(int move)
{
	return false;
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

//MoveList Knight::getAvailibleMoves(Position& position)
//{
//	MoveList list;
//	RawMoves rawMoves;
//
//	U64 knights = board_;
//	U64 blockers = getBlockerPieces(position);
//	U64 opposite = getOppositePieces(position);
//	vector<vector<int>> figureFromCoord = position.getFigureFromCoord();
//
//	U64 moves;
//	int square;
//
//	while (knights)
//	{
//		square = BitScanForward(knights);
//		rawMoves = getMoveBoards(square, blockers, opposite);
//
//		list.add(figureFromCoord, rawMoves, square, color_, KNIGHT);
//
//		knights &= knights - 1;
//	}
//	return list;
//}



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

MoveList Rook::getAvailibleMoves(Position& postiion)
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

