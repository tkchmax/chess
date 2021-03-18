#pragma once

#ifndef _GAME_
#define _GAME_

#include "Position.h"

class Game : public Position
{
public:
	Game();

	void makeMove(int move);
	void undoMove(int move);

	int evaluate(int color);

	int getMaterialEval(int color);
	int getStrategyEval(int color);
	int getMobilityEval(int color);

	bool isGameOver();

	void setIsKingMoved(int color, bool isMoved);
	void setIsLshRookMoved(int color, bool isMoved);
	void setIsRshRookMoved(int color, bool isMoved);

	string saveNotationOfMove(int move);

	string getPGN();



protected:
	void _makeShortCastling(int color);
	void _makeLongCastling(int color);
	void _makePawnTransform(int transform_in, int square, int color);
	
	void _undoShortCastling(int color);
	void _undoLongCastling(int color); 
	void _undoPawnTransform(int move_from, int transform_in, int square, int color);

	string toCoord(int number);


protected:
	bool game_over;
private:
	vector<int> firstKingMove_;
	vector<int> firstLshRookMove_;
	vector<int> firstRshRookMove_;

	vector<string> rawPGN_;


};

#endif // !_GAME_
;