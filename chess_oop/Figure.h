#pragma once

#ifndef _FIGURE_
#define _FIGURE_

#include "headers.h"
#include "MoveList.h"
#include "Position.h";

class Position;

void initializeRays();

class Figure
{
public:
	virtual MoveList getAvailibleMoves(Position& position) = 0;
	virtual RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) = 0;
	virtual U64 getAttackBoard(U64 blockers, U64 opposite);

	void setBoard(U64 board);
	void setColor(int color);

	U64 getBlockerPieces(Position& position);
	U64 getOppositePieces(Position& position);
	U64 getBoard();
	U64 getColor();

	
protected:
	U64 board_;
	int nFigures_;
	int color_;
};


class Pawn : public Figure
{
public:
	Pawn(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;
	U64 getAttackBoard(U64 blockers, U64 opposite) override;
};

class Knight : public Figure
{
public:
	Knight(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;

};

class Bishop : public Figure
{
public:
	Bishop(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;

};

class Rook : public Figure
{
public:
	Rook(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;

private:
	bool isMoved;
};

class Queen : public Figure
{
public:
	Queen(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;

};

class King : public Figure
{
public:
	King(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;
	MoveList getAvailibleMoves(Position& position) override;

private:
	bool isChecked;
	bool isMoved;


}; 
#endif // !_FIGURE_

int BitScanForward(U64 bb);
int BitScanReverse(U64 bb);
