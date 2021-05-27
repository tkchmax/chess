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
	case 5:
		return "PAWN_TO_KNIGHT";
	case 6:
		return "PAWN_TO_BISHOP";
	case 7:
		return "PAWN_TO_ROOK";
	case 8:
		return "PAWN_TO_QUEEN";
	default:
		cout << "";
		break;
	}
}

void ShowListItem(int item)
{
	string from = ToCoord(READ_FROM(item));
	string to = ToCoord(READ_TO(item));
	string figure = ConvertFigureName(READ_FIGURE(item));
	string capture = ConvertFigureName(READ_CAPTURE(item));
	string move_type = ConvertMoveType(READ_MOVE_TYPE(item));
	int listId = READ_LISTID(item);

	cout << "From: " << from << endl;
	cout << "To: " << to << endl;
	cout << "Figure to move: " << figure << endl;
	cout << "Capture figure: " << capture << endl;
	cout << "Move type: " << move_type << endl;
	cout << "Color: " << READ_COLOR(item) << endl;
	cout << "List id: " << listId << endl;
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

void operator+=(MoveList& list, int move)
{
	move &= ~(127 << 24); //clear old id
	move |= WRITE_LISTID(list.size() + 1, move);
	if (READ_MOVE_TYPE(move) == MOVE_TYPE_TAKE)
		list.capture.push_back(move);
	else 
		list.silent.push_back(move);
}

void operator+=(MoveList& lsh, const MoveList& rsh)
{
	//copy(rsh.capture.begin(), rsh.capture.end(), back_inserter(lsh.capture));
	//copy(rsh.silent.begin(), rsh.silent.end(), back_inserter(lsh.silent));
	for (int i = 0; i < rsh.capture.size(); ++i)
		lsh += rsh.capture[i];
	for (int i = 0; i < rsh.silent.size(); ++i)
		lsh += rsh.silent[i];
}

bool operator==(const MoveList& lsh, const MoveList& rsh)
{
	return (lsh.silent == rsh.silent) && (lsh.capture == rsh.capture);
}


int MoveList::next()
{
	if (!killer.empty())
	{
		int temp = killer[0];
		killer.pop_front();
		return temp;
	}
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

bool MoveList::empty() const
{
	return silent.empty() & capture.empty();
}

bool RawMoves::empty()
{
	if (silents == 0 && takes == 0)
		return true;
	return false;
}

int RawMoves::count(int type)
{
	U64 t_silents = silents;
	U64 t_takes = takes;

	int count = 0;
	if (type != MOVE_TYPE_TAKE)
	{
		while (t_silents)
		{
			count++;
			t_silents &= t_silents - 1;
		}
	}
	if (type != MOVE_TYPE_SILENT)
	{
		while (t_takes)
		{
			count++;
			t_takes &= t_takes - 1;
		}
	}
	return count;

}
