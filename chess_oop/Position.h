#pragma once
#ifndef _GAME_
#define _GAME_

#include "headers.h"
#include "Figure.h"

class Figure;

class Position
{
public:
	Position();
	
	vector<vector<shared_ptr<Figure>>> getFigures();
	vector<vector<int>> getFigureFromCoord();

	U64 getWhiteFiguresBoard();
	U64 getBlackFiguresBoard();
	U64 getAllFiguresBoard();

	MoveList getMovesList(int color);

	void setFigures(vector<vector<shared_ptr<Figure>>> figures);
	void setFigureFromCoord(vector<vector<int>> figuresFromCoord);

protected:
	MoveList getFigureMoveList(int figure, int color);

private:
	vector<vector<shared_ptr<Figure>>> figures_;
	vector<vector<int>> figureFromCoord_;
	MoveList whiteMovesList_;
	MoveList blackMovesList_;
};
#endif // !_GAME_

