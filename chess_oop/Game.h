#pragma once

#ifndef _GAME_
#define _GAME_

#include "Position.h"

class Game : public Position
{
public:

	void makeMove(int move);

protected:
	bool _makeShortCastling(int color);
	bool _makeLongCastling(int color);
	void _makePawnTransform(int transform_in, int square, int color);

private:
	bool isKingMoved_;
	bool isLshRookMoved_;
	bool isRshRookMoved_;

};

#endif // !_GAME_
