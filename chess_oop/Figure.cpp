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

int Figure::getFigureCount()
{
	return nFigures_;
}

int Figure::getCost()
{
	return pieceCost_;
}

int Figure::getPriorityEvalOnSquare(int square)
{
	return prioritySquares_[square];
}

const vector<int>& Figure::getPrioritySquares()
{
	return prioritySquares_;
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
		nFigures_--;
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
	nFigures_++;
	return true;

}

vector<int> Figure::getPiecesSquares()
{
	vector<int> squares;
	squares.reserve(10);
	U64 t_board = board_;

	int count = 0;
	int square;
	while (t_board)
	{
		square = BitScanForward(t_board);
		squares.push_back(square);
		t_board &= ~(1ULL << square);
	}
	return squares;
}


Pawn::Pawn(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEPAWN_STARTPOSITION : BLACKPAWN_STARTPOSITION);
	nFigures_ = 8;
	pieceCost_ = 100;

	vector<int> t_whitePrioritySquares = { 0, 0, 0, 0, 0, 0, 0, 0,
											4, 4, 4, 0, 0, 4, 4, 4,
											6, 8, 2, 10,10,2, 8, 6,
											6, 8, 12,16,16,12,8, 6,
											6, 12,16,24,24,16,12,8,
											12, 16,24,32,32,24,16,12,
											15, 20,30,35,35,30,20,15,
											0, 0, 0, 0, 0, 0, 0, 0 };
	vector<int> t_blackPrioritySquares = { 0, 0, 0, 0, 0, 0, 0, 0,
											15, 20,30,35,35,30,20,15,
											12, 16,24, 32,32,24,16,12,
											6,  12,16, 24,24,16,12, 8,
											6,	8, 12, 16,16,12, 8, 6,
											6,  8, 2,  10,10, 2, 8, 6,
											4,  4, 4,  0,  0, 4, 4, 4,
											0,  0, 0,  0,  0, 0, 0, 0 };
	prioritySquares_ = (color == WHITE ? t_whitePrioritySquares : t_blackPrioritySquares);
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
	U64 silentMoves = 0, captureMoves = 0;
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


Knight::Knight(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKNIGHT_STARTPOSITION : BLACKKNIGHT_STARTPOSITION);
	nFigures_ = 2;
	pieceCost_ = 300;
	prioritySquares_ = { 0, 2, 8, 10, 10, 8, 2, 0,
						4 ,8, 16, 20, 20, 16, 8, 4,
						8, 16, 25, 28, 28, 25, 16,8,
						10, 20,28, 32, 32, 28, 20,10,
						10, 20,28, 32, 32, 28, 20,10,
						8, 16, 25, 28, 28, 25, 16, 8,
						4, 8, 16, 20, 20, 16,  8,  4,
						0, 2, 8,  10, 10  ,8  ,2  ,0 };
}

RawMoves Knight::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	RawMoves moves;
	U64 board = 0;
	board |= (1ULL << (square + 10)) & (NOT_B_FILE & NOT_A_FILE);
	board |= (1ULL << (square - 6))& (NOT_B_FILE & NOT_A_FILE);
	board |= (1ULL << (square - 15)) & NOT_A_FILE;
	board |= (1ULL << (square - 17)) & NOT_H_FILE;
	board |= (1ULL << (square - 10)) & (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << (square + 6))& (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << (square + 15)) & NOT_H_FILE;
	board |= (1ULL << (square + 17)) & NOT_A_FILE;

	moves.silents = board & ~opposite & ~blockers;
	moves.takes = board & opposite;

	return moves;
}


Bishop::Bishop()
{
}

Bishop::Bishop(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEBISHOP_STARTPOSITION : BLACKBISHOP_STARTPOSITION);
	nFigures_ = 2;
	pieceCost_ = 301;
	prioritySquares_ = { 0,	0,	0,	0,	0,	0,	0,	0,
						5,  23,  15,  15,  15,  15,  23,5,
						5,  18,  22, 22, 22,  22,  18,5,
						5,  18,  22, 22, 22,  22,   18,5,
						5,  18, 22, 22, 22,  22,   18,5,
						5, 18,22, 22, 22,   22, 18,	5,
						5,  23,  15,  15,  15,  15,  23,5,
						0,	0,	0,	0,	0,	0,	0,	0, };
}

RawMoves Bishop::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	U64 moves = 0;
	U64 all_pieces = blockers | opposite;
	//North-East
	moves |= rays[NORTH_EAST][square];
	if (rays[NORTH_EAST][square] & all_pieces)
	{
		int blockerId = BitScanForward(rays[NORTH_EAST][square] & all_pieces);
		moves &= ~rays[NORTH_EAST][blockerId];
	}

	//North-West
	moves |= rays[NORTH_WEST][square];
	if (rays[NORTH_WEST][square] & all_pieces)
	{
		int blockerId = BitScanForward(rays[NORTH_WEST][square] & all_pieces);
		moves &= ~rays[NORTH_WEST][blockerId];
	}

	//South-East
	moves |= rays[SOUTH_EAST][square];
	if (rays[SOUTH_EAST][square] & all_pieces)
	{
		int blockerId = BitScanReverse(rays[SOUTH_EAST][square] & all_pieces);
		moves &= ~rays[SOUTH_EAST][blockerId];
	}

	//South-West
	moves |= rays[SOUTH_WEST][square];
	if (rays[SOUTH_WEST][square] & all_pieces)
	{
		int blockerId = BitScanReverse(rays[SOUTH_WEST][square] & all_pieces);
		moves &= ~rays[SOUTH_WEST][blockerId];
	}

	RawMoves bishopMoves;
	bishopMoves.silents = moves & ~opposite & ~blockers;
	bishopMoves.takes = moves & opposite;

	return bishopMoves;
}


Rook::Rook()
{
}

Rook::Rook(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEROOK_STARTPOSITION : BLACKROOK_STARTPOSITION);
	nFigures_ = 2;
	pieceCost_ = 500;
	prioritySquares_ = {0, 0, 0,  5,  5, 5, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  0,  0, 0, 0, 0,
						0, 0, 0,  5,  5, 5, 0, 0 };

}

RawMoves Rook::getMoveBoards(int square, U64 blockers, U64 opposite)
{

	U64 moves = 0;
	U64 all_pieces = blockers | opposite;
	//North
	moves |= rays[NORTH][square];
	if (rays[NORTH][square] & all_pieces)
	{
		int blockerId = BitScanForward(rays[NORTH][square] & all_pieces);
		moves &= ~rays[NORTH][blockerId];
	}

	//West
	moves |= rays[WEST][square];
	if (rays[WEST][square] & all_pieces)
	{
		int blockerId = BitScanReverse(rays[WEST][square] & all_pieces);
		moves &= ~rays[WEST][blockerId];
	}
	//South
	moves |= rays[SOUTH][square];
	if (rays[SOUTH][square] & all_pieces)
	{
		int blockerId = BitScanReverse(rays[SOUTH][square] & all_pieces);
		moves &= ~rays[SOUTH][blockerId];
	}
	//East
	moves |= rays[EAST][square];
	if (rays[EAST][square] & all_pieces)
	{
		int blockerId = BitScanForward(rays[EAST][square] & all_pieces);
		moves &= ~rays[EAST][blockerId];
	}

	RawMoves rookMoves;
	rookMoves.silents = moves & ~opposite & ~blockers;
	rookMoves.takes = moves & opposite;

	return rookMoves;
}

Queen::Queen(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEQUEEN_STARTPOSITION : BLACKQUEEN_STARTPOSITION);
	nFigures_ = 1;
	pieceCost_ = 1000;
	prioritySquares_ = { -20,-10,-10, 0, 0,-10,-10,-20,
						-10,  0,  0,  0,  0,  0,  0,-10,
						-10,  0,  5,  5,  5,  5,  0,-10,
						 -5,  0,  5,  5,  5,  5,  0, -5,
						 -5,  0,  5,  5,  5,  5,  0, -5,
						-10,  5,  5,  5,  5,  5,  0,-10,
						-10,  0,  0,  0,  0,  0,  0,-10,
						-20,-10,-10, 0, 0,-10,-10,-20 };
}

RawMoves Queen::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	RawMoves bishopMoves = Bishop::getMoveBoards(square, blockers, opposite);
	RawMoves rookMoves = Rook::getMoveBoards(square, blockers, opposite);

	RawMoves queenMoves;
	queenMoves.silents = bishopMoves.silents | rookMoves.silents;
	queenMoves.takes = bishopMoves.takes | rookMoves.takes;

	return queenMoves;

}


King::King(int color)
{
	color_ = color;
	board_ = (color == WHITE ? WHITEKING_STARTPOSITION : BLACKKING_STARTPOSITION);
	nFigures_ = 1;
	pieceCost_ = 6000;

	vector<int> t_blackKingMiddleGamePriority = { -30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											 0, 0, -28,  -30,  -30, -28, 0, 0 };
	vector<int> t_whiteKingMiddleGamePriority = { 0, 0, -28,  -30,  -30, -28, 0, 0,
											 -30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30, };
	vector<int> t_kingEndGamePriority ={0, 0, 0,  0,  0, 0, 0, 0,
										0, 3,  3,  3,  3,  3, 3, 0,
										5,10,10,10,10,10,10, 5,
										10,20,20,30,30,20,20, 10,
										10,20,20,30,30,20,20, 10,
										5,-40,-40,-50,-50,-40,-40, 5,
										0, 3,  3,  3,  3,  3, 3, 0,
										0, 0, 0,  0,  0, 0, 0, 0 };

	prioritySquares_ = (color == WHITE ? t_whiteKingMiddleGamePriority : t_blackKingMiddleGamePriority);
}

RawMoves King::getMoveBoards(int square, U64 blockers, U64 opposite)
{
	U64 moves = 0;
	moves |= (1ULL << (square + 1)) & NOT_A_FILE; //east
	moves |= (1ULL << (square + 9)) & NOT_A_FILE; //north-east
	moves |= (1ULL << (square - 7)) & NOT_A_FILE; //south_east
	moves |= (1ULL << (square - 1)) & NOT_H_FILE; //west
	moves |= (1ULL << (square - 9)) & NOT_H_FILE; //south-west
	moves |= (1ULL << (square + 7)) & NOT_H_FILE; //north-west
	moves |= (1ULL << (square + 8)); // north
	moves |= (1ULL << (square - 8)); // south


	RawMoves kingMoves;
	kingMoves.silents = moves & ~opposite & ~blockers;
	kingMoves.takes = moves & opposite;

	return kingMoves;
}


