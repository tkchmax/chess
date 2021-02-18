#include "Figure.h"

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
	U64 blockers = (color_ == WHITE ? position.getWhiteFiguresBoard() : position.getBlackFiguresBoard());
	return blockers;
}

U64 Figure::getOppositePieces(Position& position)
{
	U64 opposite = (color_ == WHITE ? position.getBlackFiguresBoard() : position.getWhiteFiguresBoard());
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



Pawn::Pawn(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEPAWN_STARTPOSITION : BLACKPAWN_STARTPOSITION);
}

RawMoves Pawn::getWhiteAvailibleMove(int square, U64 blockers, U64 opposite)
{
	RawMoves moves;

	U64 allPieces = blockers | opposite;

	U64 figure_bit = 1ULL << square;

	U64 oneSquareMove = figure_bit << 8;
	U64 twoSqaureMove = figure_bit << 16;
	U64 leftAttack = (figure_bit << 9) & NOT_A_FILE;
	U64 rightAttack = (figure_bit << 7) & NOT_H_FILE;

	bool isBlockerInFront = (oneSquareMove)&allPieces;
	bool isPawnOnStartingRank = figure_bit & WHITEPAWN_STARTPOSITION;
	if (!isBlockerInFront && isPawnOnStartingRank)
		moves.silents |= twoSqaureMove & ~allPieces;
	moves.silents |= oneSquareMove & ~allPieces;

	if (leftAttack & opposite) moves.takes |= leftAttack;
	if (rightAttack & opposite) moves.takes |= rightAttack;

	return moves;
}

RawMoves Pawn::getBlackAvailibleMove(int square, U64 blockers, U64 opposite)
{
	RawMoves moves;

	U64 allPieces = blockers | opposite;

	U64 figure_bit = 1ULL << square;

	U64 oneSquareMove = figure_bit >> 8;
	U64 twoSqaureMove = figure_bit >> 16;
	U64 leftAttack = (figure_bit >> 7)& NOT_A_FILE;
	U64 rightAttack = (figure_bit >> 9)& NOT_H_FILE;

	bool isBlockerInFront = (oneSquareMove)&allPieces;
	bool isPawnOnStartingRank = figure_bit & BLACKPAWN_STARTPOSITION;
	if (!isBlockerInFront && isPawnOnStartingRank)
		moves.silents |= twoSqaureMove & ~allPieces;
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
		if (color_ == WHITE)
			t_moves = getWhiteAvailibleMove(square, blockers, opposite);
		else
			t_moves = getBlackAvailibleMove(square, blockers, opposite);
		list.add(figureFromCoord, t_moves, square, color_, PAWN);

		pawns &= pawns - 1;
	}
	return list;
}



Knight::Knight(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKNIGHT_STARTPOSITION : BLACKKNIGHT_STARTPOSITION);
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
		moves = 0;
		square = BitScanForward(knights);

		moves |= (1ULL << square + 10) & (NOT_B_FILE & NOT_A_FILE);
		moves |= (1ULL << square - 6) & (NOT_B_FILE & NOT_A_FILE);
		moves |= (1ULL << square - 15) & NOT_A_FILE;
		moves |= (1ULL << square - 17) & NOT_H_FILE;
		moves |= (1ULL << square - 10) & (NOT_G_FILE & NOT_H_FILE);
		moves |= (1ULL << square + 6) & (NOT_G_FILE & NOT_H_FILE);
		moves |= (1ULL << square + 15) & NOT_H_FILE;
		moves |= (1ULL << square + 17) & NOT_A_FILE;

		rawMoves.silents = moves & ~opposite & ~blockers;
		rawMoves.takes = moves & opposite;

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

MoveList Bishop::getAvailibleMoves(Position& game)
{
	return MoveList();
}



Rook::Rook(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEROOK_STARTPOSITION : BLACKROOK_STARTPOSITION);
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

MoveList Queen::getAvailibleMoves(Position& game)
{
	return MoveList();
}



King::King(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKING_STARTPOSITION : BLACKKING_STARTPOSITION);
}

MoveList King::getAvailibleMoves(Position& game)
{
	return MoveList();
}
