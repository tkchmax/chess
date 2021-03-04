#include "Figure.h"

extern vector<vector<U64>> rays;


Figure::Figure() : name_(-1), nFigures_(-1), color_(-1), pieceCost_(-1), board_(0)
{
}

MoveList Figure::getAvailibleMoves(const Position& position)
{
	MoveList list;
	int oppositeColor = (color_ == WHITE) ? BLACK : WHITE;

	U64 board = board_;


	//U64 blockers = getBlockerPieces(position);
	//U64 opposite = getOppositePieces(position);
	U64 blockers = position.getSideBoard(color_);
	U64 opposite = position.getSideBoard(oppositeColor);

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

			move = CreateListItem(from, to, name_, 0, MOVE_TYPE_SILENT, color_);
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


int Figure::getName()
{
	return name_;
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
	name_ = PAWN;
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
	U64 startPosition = 0;

	if (color_ == WHITE)
	{

		oneSquareMove = figure_bit << 8;
		twoSquareMove = figure_bit << 16;
		leftAttack = (figure_bit << 9) & NOT_A_FILE;
		rightAttack = (figure_bit << 7) & NOT_H_FILE;
		startPosition = WHITEPAWN_STARTPOSITION;
	}
	else {
		oneSquareMove = figure_bit >> 8;
		twoSquareMove = figure_bit >> 16;
		leftAttack = (figure_bit >> 7)& NOT_A_FILE;
		rightAttack = (figure_bit >> 9)& NOT_H_FILE;
		startPosition = BLACKPAWN_STARTPOSITION;
	}

	bool isBlockerInFront = (oneSquareMove)&allPieces;
	bool isPawnOnStartingRank = figure_bit & startPosition;
	if (!isBlockerInFront && isPawnOnStartingRank)
		moves.silents |= twoSquareMove & ~allPieces;
	moves.silents |= oneSquareMove & ~allPieces;

	if (leftAttack & opposite) moves.takes |= leftAttack;
	if (rightAttack & opposite) moves.takes |= rightAttack;

	return moves;
}
MoveList Pawn::getAvailibleMoves(const Position& position)
{
	MoveList list;
	int oppositeColor = (color_ == WHITE) ? BLACK : WHITE;

	U64 pawns = board_;

	//U64 blockers = getBlockerPieces(position);
	//U64 opposite = getOppositePieces(position);
	U64 blockers = position.getSideBoard(color_);
	U64 opposite = position.getSideBoard(oppositeColor);

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

		move = 0;
		while (silentMoves)
		{
			U64 t1 = board_;
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
	name_ = KNIGHT;
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
	board |= (1ULL << (square - 6)) & (NOT_B_FILE & NOT_A_FILE);
	board |= (1ULL << (square - 15)) & NOT_A_FILE;
	board |= (1ULL << (square - 17)) & NOT_H_FILE;
	board |= (1ULL << (square - 10)) & (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << (square + 6)) & (NOT_G_FILE & NOT_H_FILE);
	board |= (1ULL << (square + 15)) & NOT_H_FILE;
	board |= (1ULL << (square + 17)) & NOT_A_FILE;

	moves.silents = board & ~opposite & ~blockers;
	moves.takes = board & opposite;

	return moves;
}


Bishop::Bishop()
{
	name_ = BISHOP;
}
Bishop::Bishop(int color)
{
	name_ = BISHOP;
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

void ShowBits1(U64 bb)
{
	int bit;
	int count = 0;

	int i = 64 - 8;
	for (; i >= 0; i++) {
		bit = (bb >> i) & 1;
		cout << bit << '\t';
		count++;
		if (count == 8)
		{
			count = 0;
			i = i - 16;
			cout << endl;
		}
	}
	cout << endl << endl;
}
Rook::Rook()
{
	name_ = ROOK;
}
Rook::Rook(int color)
{
	name_ = ROOK;
	color_ = color;
	board_ = (color == WHITE ? WHITEROOK_STARTPOSITION : BLACKROOK_STARTPOSITION);
	nFigures_ = 2;
	pieceCost_ = 500;
	prioritySquares_ = { 0, 0, 0,  5,  5, 5, 0, 0,
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
	name_ = QUEEN;
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
	name_ = KING;
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
	vector<int> t_kingEndGamePriority = { 0, 0, 0,  0,  0, 0, 0, 0,
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


