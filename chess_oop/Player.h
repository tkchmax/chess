#pragma once
#ifndef  _PLAYER_
#define _PLAYER_

#include "Game.h"


class Player
{
public:
	Player(Game* game, int color);
	int alphaBeta(int depth, int alpha, int beta);
	void applyMove(int move);
private:
	Game* game_;
	int color_;
	int _alphaBeta(int depth, int alpha, int beta, int color);
	int _quies(int alpha, int beta, int color);
	void _addKillerMove(int ply, int move);
	void _sortMoveList(int ply, MoveList& moveList);

	vector<vector<int>> killerMoves_;
};


#endif
