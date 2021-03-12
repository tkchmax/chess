#pragma once

#ifndef _FIGURE_
#define _FIGURE_

#include "headers.h"
#include "MoveList.h"
#include "Position.h";

class Position;

class Figure
{
public:
	Figure();
	virtual RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) = 0;
	virtual MoveList getAvailibleMoves(const Position& position, int type=ALL);
	virtual U64 getAttackBoard(U64 blockers, U64 opposite);

	void setBoard(U64 board);
	void setColor(int color);
	void setCount(int count);

	int getName();
	U64 getBoard();
	U64 getColor();
	int getCount();
	int getCost();
	int getPriorityEvalOnSquare(int square);
	const vector<int>& getPrioritySquares();

	bool moveFigure(int from_square, int to_square);
	bool removePiece(int square);
	bool setFigureOnSquare(int square);

	vector<int> getPiecesSquares();

protected:
	int name_;
	U64 board_;
	int nFigures_;
	int color_;

	int pieceCost_;
	vector<int> prioritySquares_;

};


class Pawn : public Figure
{
public:
	Pawn(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(const Position& position, int type=ALL) override;
	U64 getAttackBoard(U64 blockers, U64 opposite) override;
};

class Knight : public Figure
{
public:
	Knight(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;

};

class Bishop : public virtual Figure
{
public:
	Bishop();
	Bishop(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
};

class Rook : public virtual Figure
{
public:
	Rook();
	Rook(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
};

class Queen : public Bishop, public Rook
{
public:
	Queen(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
};

class King : public Figure
{
public:
	King(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
}; 


#endif // !_FIGURE_

int BitScanForward(U64 bb);
int BitScanReverse(U64 bb);

