#include "Position.h"
//#include "Figure.h"
#include <sstream>
#include "Game.h"

void ShowBits(U64 bb)
{
	int bit;
	int count = 0;

	int i = 64 - 8;
	for (; i >= 0; i++) {
		bit = (bb >> i) & 1;
		cout << bit << '\t';
		count++;
		if (count == 8)
		{
			count = 0;
			i = i - 16;
			cout << endl;
		}
	}
	cout << endl << endl;
}

vector<vector<U64>> rays(8, vector<U64>(64));
U64 _aOut(U64 bb)
{
	return (bb << 1) & NOT_A_FILE;
}
U64 _hOut(U64 bb)
{
	return(bb >> 1)& NOT_H_FILE;
}

void _initializeRays()
{

	U64 north = 0x0101010101010100;
	U64 west = 9151314442816847872;
	U64 south = 0x0080808080808080;
	U64 east = 254;
	U64 north_east = 0x8040201008040200;
	U64 north_west = 72624976668147712;
	U64 south_west = 18049651735527937;
	U64 south_east = 567382630219904;

	//North
	for (int i = 0; i < 64; ++i, north <<= 1)
	{
		rays[NORTH][i] = north;
	}

	//West
	for (int x = 0; x < 8; ++x)
	{
		U64 w = west;
		west = _hOut(west);
		for (int y = 63; y >= 0; y -= 8, w >>= 8)
		{
			rays[WEST][y - x] = w;
		}
	}

	//South
	for (int i = 63; i >= 0; --i, south >>= 1)
	{
		rays[SOUTH][i] = south;
	}

	//East
	for (int x = 0; x < 8; ++x)
	{
		U64 e = east;
		east = _aOut(east);
		for (int y = 0; y < 64; y += 8, e <<= 8)
		{
			rays[EAST][x + y] = e;
		}
	}

	//North-East
	for (int y = 0; y < 8; ++y)
	{
		U64 ne = north_east;
		north_east = _aOut(north_east);
		for (int x = 0; x < 64; x += 8, ne <<= 8)
		{
			rays[NORTH_EAST][x + y] = ne;
		}
	}

	//North-West
	for (int x = 7; x >= 0; --x)
	{
		U64 nw = north_west;
		north_west = _hOut(north_west);
		for (int y = 0; y < 64; y += 8, nw <<= 8)
		{
			rays[NORTH_WEST][y + x] = nw;
		}
	}

	//South-West
	for (int x = 0; x < 8; x++)
	{
		U64 sw = south_west;
		south_west = _hOut(south_west);
		for (int y = 63; y >= 0; y -= 8, sw >>= 8)
		{
			rays[SOUTH_WEST][y - x] = sw;
		}
	}

	//South-East
	for (int x = 7; x >= 0; --x)
	{
		U64 se = south_east;
		south_east = _aOut(south_east);
		for (int y = 63; y >= 8; y -= 8, se >>= 8)
		{
			rays[SOUTH_EAST][y - x] = se;
		}
	}
}

int main()
{
	_initializeRays();

	Game game;

	int move = CreateListItem(8, 16, PAWN, 0, MOVE_TYPE_SILENT, WHITE);
	game.makeMove(move);
	
	cout << game.getMoves(BLACK);

}

