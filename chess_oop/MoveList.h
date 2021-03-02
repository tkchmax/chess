#pragma once
#include "headers.h"

#ifndef _MOVELIST_
#define _MOVELIST_

struct RawMoves
{
	U64 silents;
	U64 takes;
	RawMoves()
	{
		silents = 0;
		takes = 0;
	}
	vector<int> getSilentMoves();
};

struct MoveList
{
	int silent[100];
	int capture[100];

	int sEnd = -1;
	int cEnd = -1;
	
	friend void operator+=(MoveList& lsh, const int move);
	friend void operator+=(MoveList& lsh, const MoveList& rsh);
	friend ostream& operator<<(ostream& out, const MoveList& list);

	void add(vector<vector<int>> figureFromCoord, RawMoves moves, int from, int color, int figure);
};

int CreateListItem(int from, int to, int figure, int capture, int move_type, int color);

void ShowListItem(int item);

#endif



