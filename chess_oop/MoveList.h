#pragma once
#include "headers.h"
#include <deque>

#ifndef _MOVELIST_
#define _MOVELIST_

void ShowBits(U64 bb);
void ShowBoardVector(vector<vector<int>> board, int color);

struct RawMoves
{
	U64 silents;
	U64 takes;
	RawMoves()
	{
		silents = 0;
		takes = 0;
	}
	void operator+=(const RawMoves& moves)
	{
		silents |= moves.silents;
		takes |= moves.takes;
	}
};

struct MoveList
{
	deque<int> silent;
	deque<int> capture;
	
	friend void operator+=(MoveList& lsh, const int move);
	friend void operator+=(MoveList& lsh, const MoveList& rsh);
	friend bool operator==(const MoveList& lsh, const MoveList& rsh);
	friend ostream& operator<<(ostream& out, const MoveList& list);
	std::deque<int>::iterator captureIter;

	int next();
	int size() const;



private:
	std::deque<int>::iterator silentIter;
};

int CreateListItem(int from, int to, int figure, int capture, int move_type, int color);

void ShowListItem(int item);

#endif



