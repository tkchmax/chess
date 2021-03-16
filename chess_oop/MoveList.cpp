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

void ShowBoardVector(vector<vector<int>> board, int color)
{
	int count = 0;

	int i = 64 - 8;
	for (; i >= 0; i++) {
		//bit = (bb >> i) & 1;
		//cout << bit << '\t';
		cout << board[color][i]<<'\t';
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

ostream& operator<<(ostream& out, const MoveList& deque)
{
	int count = 0;
	out << "silent_count: " << deque.silent.size() << endl;
	out << "capture_count: " << deque.capture.size() << endl << endl;


	for (auto const& item : deque.silent)
	{
		ShowListItem(item);
		out << endl;
	}
	for (auto const& item : deque.capture)
	{
		ShowListItem(item);
		out << endl;
	}
	return out;
}

int CreateListItem(int from, int to, int figure, int capture, int move_type, int color)
{
	int item = 0;
	item = WRITE_FROM(from, item) | WRITE_TO(to, item) | WRITE_FIGURE(figure, item) | WRITE_CAPTURE(capture, item) | WRITE_MOVE_TYPE(move_type, item) | WRITE_COLOR(color, item);
	return item;
}

void operator+=(MoveList& deque, const int move)
{
	if (move == 0)
		cout << "";
	int type = READ_MOVE_TYPE(move);
	if (type == MOVE_TYPE_TAKE)
		deque.capture.push_back(move);
	else 
		deque.silent.push_back(move);
}

void operator+=(MoveList& lsh, const MoveList& rsh)
{
	copy(rsh.capture.begin(), rsh.capture.end(), back_inserter(lsh.capture));
	copy(rsh.silent.begin(), rsh.silent.end(), back_inserter(lsh.silent));
}

bool operator==(const MoveList& lsh, const MoveList& rsh)
{
	return (lsh.silent == rsh.silent) && (lsh.capture == rsh.capture);
}


int MoveList::next()
{

	if (!capture.empty())
	{	
		int temp = capture[0];
		capture.pop_front();
		return temp;
	}
	if (!silent.empty())
	{
		int temp = silent[0];
		silent.pop_front();
		return temp;
	}

	return 0;
}

int MoveList::size() const
{
	return silent.size() + capture.size();

}
