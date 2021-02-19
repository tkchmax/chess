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

	U64 getFigureBoard(int color);
	U64 getAllFiguresBoard();

	bool isMoveLegal(int move);
	U64 getAtackRays(int color);

	void setFigures(vector<vector<shared_ptr<Figure>>> figures);
	void setFigureFromCoord(vector<vector<int>> figuresFromCoord);

	MoveList getFigureMoveList(int figure, int color);

private:
	vector<vector<shared_ptr<Figure>>> figures_;
	vector<vector<int>> figureFromCoord_;
};
#endif // !_GAME_

