#pragma once
#ifndef _POSITION_
#define _POSITION_

#include "Figure.h"
#include <algorithm>
#include <sstream>

class Figure;

class Position
{
public:
	Position();

	Position(const Position& position);

	bool operator==(const Position& position);
	
	vector<vector<shared_ptr<Figure>>> getFigures() const;
	vector<vector<int>> getFigureFromCoord() const;

	U64 getSideBoard(int color) const;
	U64 getAllFiguresBoard();


	bool isMoveLegal(int move) const ;
	U64 getAtackRays(int color) const;

	void setFigures(vector<vector<shared_ptr<Figure>>> figures);
	void setFigureFromCoord(vector<vector<int>> figuresFromCoord);
	void setFigureFromCoord(int color, int square, int figure) { figureFromCoord_[color][square] = figure; }

	bool isKingAttacked(int color) const;
	//bool isKingAttacked(const U64& kingBoard, U64& oppositePieces, U64& blockers);
	bool isCastlingPossible(int color, int castlingType) const;

	MoveList getFigureMoveList(int figure, int color) const;
	MoveList getMoves(int color, int type=ALL) const;
	U64 getFigureBoard(int figure, int color) const;
	shared_ptr<Figure> getFigure(int figure, int color);

	int getFigureOnSquare(int square, int color) const;
	int getSideFiguresCount(int color) const;

	int getFigureCount(int figure, int color);

	void empty();

	void setFEN(string fen);
	string getFEN();
	string getFigureIdOnSquare(int square);

	bool isKingMoved(int color) { return isKingMoved_[color]; }
	bool isLshRookMoved(int color) { return isLshRookMoved_[color]; }
	bool isRshRookMoved(int color) { return isRshRookMoved_[color]; }

protected:
	vector<vector<shared_ptr<Figure>>> figures_;
	vector<vector<int>> figureFromCoord_;

	vector<bool> isKingMoved_;
	vector<bool> isLshRookMoved_;
	vector<bool> isRshRookMoved_;

private:
	//Position(const Position& position);

};
#endif // !_POSITION_

