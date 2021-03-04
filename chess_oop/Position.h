#pragma once
#ifndef _POSITION_
#define _POSITION_

#include "headers.h"
#include "Figure.h"

class Figure;

class Position
{
public:
	Position();
	
	vector<vector<shared_ptr<Figure>>> getFigures() const;
	vector<vector<int>> getFigureFromCoord() const;

	U64 getSideBoard(int color) const;
	U64 getAllFiguresBoard();


	bool isMoveLegal(int move) const;
	U64 getAtackRays(int color) const;

	void setFigures(vector<vector<shared_ptr<Figure>>> figures);
	void setFigureFromCoord(vector<vector<int>> figuresFromCoord);

	bool isKingAttacked(int color) const;

	MoveList getFigureMoveList(int figure, int color) const;
	MoveList getMoves(int color) const;
	U64 getFigureBoard(int figure, int color) const;

	int getFigureOnSquare(int square) const ;
	int getSideFiguresCount(int color) const;


protected:
	vector<vector<shared_ptr<Figure>>> figures_;
	vector<vector<int>> figureFromCoord_;
private:
	Position(const Position& position);

};
#endif // !_GAME_

