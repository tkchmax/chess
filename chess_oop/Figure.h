#pragma once

#ifndef _FIGURE_
#define _FIGURE_

#include "headers.h"
#include "MoveList.h"
#include "Position.h";
//typedef vector<vector<Figure*>> pFiguresArray;

class Position;

class Figure
{
public:
	virtual MoveList getAvailibleMoves(Position& position) = 0;

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
	RawMoves getWhiteAvailibleMove(int sqaure, U64 blockers, U64 opposite);
	RawMoves getBlackAvailibleMove(int square, U64 blockers, U64 opposite);
	MoveList getAvailibleMoves(Position& game) override;
};

class Knight : public Figure
{
public:
	Knight(int color);
	MoveList getAvailibleMoves(Position& position) override;

};

class Bishop : public Figure
{
public:
	Bishop(int color);
	MoveList getAvailibleMoves(Position& position) override;


};

class Rook : public Figure
{
public:
	Rook(int color);
	MoveList getAvailibleMoves(Position& position) override;


};

class Queen : public Figure
{
public:
	Queen(int color);
	MoveList getAvailibleMoves(Position& position) override;


};

class King : public Figure
{
public:
	King(int color);
	MoveList getAvailibleMoves(Position& position) override;


};

#endif // !_FIGURE_

int BitScanForward(U64 bb);

int BitScanReverse(U64 bb);
