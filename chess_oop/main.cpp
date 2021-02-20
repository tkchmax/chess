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


int main()
{
	Position pos;

	Game game;

	ShowBits(game.getSideBoard(WHITE));
	int move = CreateListItem(8, 24, PAWN, 0, MOVE_TYPE_SILENT, WHITE);
	
	game.makeMove(move);

	ShowBits(game.getSideBoard(WHITE));


	


}

