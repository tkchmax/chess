#pragma once

#ifndef _FIGURE_
#define _FIGURE_

#include "headers.h"
#include "MoveList.h"
#include "Position.h";

class Position;

void _initializeRays();

class Figure
{
public:
	virtual RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) = 0;
	virtual MoveList getAvailibleMoves(Position& position);
	virtual U64 getAttackBoard(U64 blockers, U64 opposite);

	void setBoard(U64 board);
	void setColor(int color);

	U64 getBlockerPieces(Position& position);
	U64 getOppositePieces(Position& position);
	U64 getBoard();
	U64 getColor();

	bool moveFigure(int old_square, int new_square);
	bool removePiece(int square);
	bool setFigureOnSquare(int square);
	
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
private:
	bool _isPawnTransformMove(int move);
};

class Knight : public Figure
{
public:
	Knight(int color);
	RawMoves getMoveBoards(int square, U64 blockers, U64 opposite) override;

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


}; 
#endif // !_FIGURE_

int BitScanForward(U64 bb);
int BitScanReverse(U64 bb);
