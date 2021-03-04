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

protected:
	bool _makeShortCastling(int color);
	bool _makeLongCastling(int color);
	void _makePawnTransform(int transform_in, int square, int color);
	
	bool _undoShortCastling(int color);
	bool _undoLongCastling(int color); 
	void _undoPawnTransform(int transform_in, int square, int color);

protected:
	vector<int> isKingMoved_;
	vector<int> isLshRookMoved_;
	vector<int> isRshRookMoved_;

};

#endif // !_GAME_
;