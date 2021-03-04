#include "MoveList.h"
#include <sstream>

int BitScanForward(U64 bb)
{
	//¬Œ«¬–¿Ÿ¿≈Ã »Õƒ≈ — œ≈–¬Œ√Œ «Õ¿◊¿Ÿ≈√Œ ¡»“¿
	U64 mask = 1ULL;
	if (bb)
	{
		for (int i = 0; i < 64; ++i)
		{
			if (bb & mask) return i;
			mask <<= 1;
		}
	}
	return -1;
}
int BitScanReverse(U64 bb)//??
{
	//¬Œ«¬–¿Ÿ¿≈Ã »Õƒ≈ — œŒ—À≈ƒÕ≈√Œ «Õ¿◊¿Ÿ≈√Œ ¡»“¿
	U64 mask = pow(2, 63);
	if (bb)
	{
		for (int i = 0; i < 64; ++i)
		{
			if (bb & mask) return 63 - i;
			mask >>= 1;
		}
	}
	return -1;
}


string ConvertFigureName(int number)
{
	switch (number)
	{
	case 0:
		return "NO_FIGURE";
	case 1:
		return "PAWN";
	case 2:
		return "KNIGHT";
	case 3:
		return "BISHOP";
	case 4:
		return "ROOK";
	case 5:
		return "QUEEN";
	case 6:
		return "KING";
	}
}
string ToCoord(int number)
{
	char rank = char((number / 8) + '1');
	char file = char(number % 8 + 'a');

	stringstream ss;
	ss << file << rank;

	string coord;
	ss >> coord;

	return coord;
}
string ConvertMoveType(int number)
{
	switch (number)
	{
	case 0:
		return "SILENT_MOVE";
	case 1:
		return "CAPTURE_MOVE";
	case 2:
		return "SHORT_CASTLING";
	case 3:
		return "LONG_CASTLING";
	case 4:
		return "2_SQUARE_PAWN_MOVE";
	case 6:
		return "PAWN_TRANSFORMATION";
	case 7:
		return "PAWN_TO_KNIGHT";
	case 8:
		return "PAWN_TO_BISHOP";
	case 9:
		return "PAWN_TO_ROOK";
	case 10:
		return "PAWN_TO_QUEEN";
	}
}

void ShowListItem(int item)
{
	string from = ToCoord(READ_FROM(item));
	string to = ToCoord(READ_TO(item));
	string figure = ConvertFigureName(READ_FIGURE(item));
	string capture = ConvertFigureName(READ_CAPTURE(item));
	string move_type = ConvertMoveType(READ_MOVE_TYPE(item));

	cout << "From: " << from << endl;
	cout << "To: " << to << endl;
	cout << "Figure to move: " << figure << endl;
	cout << "Capture figure: " << capture << endl;
	cout << "Move type: " << move_type << endl;
}

ostream& operator<<(ostream& out, const MoveList& list)
{
	int count = 0;
	out << "silent_count: " << list.sEnd + 1 << endl;
	out << "capture_count: " << list.cEnd + 1 << endl << endl;


	for (int i = 0; i <= list.sEnd; ++i)
	{
		ShowListItem(list.silent[i]);
		out << endl;


	}
	for (int j = 0; j <= list.cEnd; ++j)
	{
		ShowListItem(list.capture[j]);
		out << endl;
	}
	out << endl;
	return out;

}/*
	for (int i = 0; i <= sEnd; ++i)
	{
		if (READ_FIGURE(silent[i]) == type)
		{
			ShowListItem(mv.silent[i]);
			cout << endl;
		}
	}
	for (int j = 0; j <= cEnd; ++j)
	{
		if (READ_FIGURE(capture[j]) == type)
		{
			ShowListItem(capture[j]);
			cout << endl;
			count++;
		}
	}*/

int CreateListItem(int from, int to, int figure, int capture, int move_type, int color)
{
	int item = 0;
	item = WRITE_FROM(from, item) | WRITE_TO(to, item) | WRITE_FIGURE(figure, item) | WRITE_CAPTURE(capture, item) | WRITE_MOVE_TYPE(move_type, item) | WRITE_COLOR(color, item);
	return item;
}

void MoveList::add(vector<vector<int>> figureFromCoord, RawMoves moves, int from, int color, int figure)
{
	int oppositeColor = (color == WHITE ? BLACK : WHITE);
	int move, to, capture;

	while (moves.silents)
	{
		to = BitScanForward(moves.silents);
		move = CreateListItem(from, to, figure, NO_FIGURE, MOVE_TYPE_SILENT, color);
		*this += move;

		moves.silents &= moves.silents - 1;
	}
	while (moves.takes)
	{
		to = BitScanForward(moves.takes);
		capture = figureFromCoord[oppositeColor][to];
		move = CreateListItem(from, to, figure, capture, MOVE_TYPE_TAKE, color);
		*this += move;
		moves.takes &= moves.takes - 1;
	}
}

void operator+=(MoveList & list, const int move)
{
	int type = READ_MOVE_TYPE(move);
	if (type == MOVE_TYPE_TAKE)
	{
		list.cEnd++;
		list.capture[list.cEnd] = move;
	}
	else {
		list.sEnd++;
		list.silent[list.sEnd] = move;
	}
}

void operator+=(MoveList & lsh, const MoveList & rsh)
{
	for (int i = 0; i <= rsh.sEnd; ++i)
		lsh += rsh.silent[i];
	for (int j = 0; j <= rsh.cEnd; ++j)
		lsh += rsh.capture[j];
}

vector<int> RawMoves::getSilentMoves()
{
	return vector<int>();
}
