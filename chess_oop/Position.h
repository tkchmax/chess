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
	
	vector<vector<shared_ptr<Figure>>> getFigures();
	vector<vector<int>> getFigureFromCoord();

	U64 getSideBoard(int color);
	U64 getAllFiguresBoard();


	bool isMoveLegal(int move);
	U64 getAtackRays(int color);

	void setFigures(vector<vector<shared_ptr<Figure>>> figures);
	void setFigureFromCoord(vector<vector<int>> figuresFromCoord);

	bool isKingAttacked(int color);

	MoveList getFigureMoveList(int figure, int color);
	U64 getFigureBoard(int figure, int color);

	int getFigureOnSquare(int square);
	int getSideFiguresCount(int color);


protected:
	vector<vector<shared_ptr<Figure>>> figures_;
	vector<vector<int>> figureFromCoord_;
};
#endif // !_GAME_

