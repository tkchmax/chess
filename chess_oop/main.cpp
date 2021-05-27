#include "Player.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>

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
int to_typeId(string type)
{
	if (type == "s")
		return MOVE_TYPE_SILENT;
	else if (type == "t")
		return MOVE_TYPE_TAKE;
}
int to_square(string coord)
{
	char file = coord[0];
	char rank = coord[1];
	int x = int(file % 97);
	int y = int(rank % 49) * 8;
	return x + y;
}
int to_figureId(string figure)
{
	if (figure == "p")
		return PAWN;
	else if (figure == "n")
		return KNIGHT;
	else if (figure == "b")
		return BISHOP;
	else if (figure == "r")
		return ROOK;
	else if (figure == "q")
		return QUEEN;
	else if (figure == "k")
		return KING;
}
void UI(Game& game, int color) //temp
{
	string move_from, move_to, type;
	cout << "from >> "; cin >> move_from;
	cout << "to >> "; cin >> move_to;
	int oppositeColor = (color == WHITE) ? BLACK : WHITE;

	if (move_from == "short")
	{
		int from = (color == WHITE) ? 4 : 60;
		int to = (color == WHITE) ? 6 : 62;
		int move = CreateListItem(from, to, KING, 0, MOVE_TYPE_0_0, color);
		game.makeMove(move);
		return;
	}
	else if (move_from == "long")
	{
		int from = (color == WHITE) ? 4 : 60;
		int to = (color == WHITE) ? 2 : 58;
		int move = CreateListItem(from, to, KING, 0, MOVE_TYPE_0_0_0, color);
		game.makeMove(move);
		return;
	}

	type = "s";

	int capture = 0;
	if (game.getFigureOnSquare(to_square(move_to), oppositeColor))
	{
		capture = game.getFigureOnSquare(to_square(move_to), oppositeColor);
		type = "t";
	}

	if (move_from == "transform")
	{
		int to;
		cout << endl;
		cout << "to: "; cin >> to;
		switch (to)
		{
		case KNIGHT:
			type = PAWN_TO_KNIGHT;
			break;
		case BISHOP:
			type = PAWN_TO_BISHOP;
			break;
		case QUEEN:
			type = PAWN_TO_QUEEN;
			break;

		}
		cout << "move from >> "; cin >> move_from;
		int move = CreateListItem(to_square(move_from), to_square(move_to), PAWN, capture, 8, color);
		cout << "move\n";
		game.makeMove(move);
		return;
	}


	int figure = game.getFigureOnSquare(to_square(move_from), color);

	int move = CreateListItem(to_square(move_from), to_square(move_to), figure, capture, to_typeId(type), color);

	assert(figure < 7);

	cout << "eval before make move " << game.evaluate(color) << endl;
	game.makeMove(move);

	ShowListItem(move);
	cout << "eval after make move " << game.evaluate(color) << endl;

	cout << endl << endl;



}

int main()
{
	_initializeRays();


	Game game;
	Player p1(&game, WHITE);
	Player p2(&game, BLACK);

	//Position pos1;
	//pos1.setFEN("r6r/1p3ppp/p5k1/2P2b2/8/3P2N1/P2Q1PqP/2R1K2R/");

	//Position pos2(pos1);

	//ShowBoardVector(pos1.getFigureFromCoord(), WHITE);
	//ShowBoardVector(pos1.getFigureFromCoord(), BLACK);
	//cout << endl;
	//ShowBoardVector(pos2.getFigureFromCoord(), WHITE);
	//ShowBoardVector(pos2.getFigureFromCoord(), BLACK);

	//vector<vector<int>> f1 = pos1.getFigureFromCoord();
	//vector<vector<int>> f2 = pos2.getFigureFromCoord();
	//for (int i = 0; i < 2; ++i)
	//	for (int j = 0; j < 64; ++j)
	//		if (f1[i][j] != f2[i][j])
	//			cout << "!!!";

	//for (int i = PAWN; i <= KING; ++i)
	//	if (pos1.getFigureBoard(i, WHITE) != pos2.getFigureBoard(i, WHITE))
	//		cout << "w board!!";
	//for (int i = PAWN; i <= KING; ++i)
	//	if (pos1.getFigureBoard(i, BLACK) != pos2.getFigureBoard(i, BLACK))
	//		cout << "b board!!";

	//cout << game.getMoves(WHITE);

	//int move = CreateListItem(0, 63, ROOK, PAWN, MOVE_TYPE_TAKE, BLACK);
	//move |= WRITE_LISTID(3, move);
	
	//cout << "from " << READ_FROM(move)<<endl;
	//cout << "to " << READ_TO(move)<<endl;
	//cout << "fig " << READ_FIGURE(move)<<endl;
	//cout << "capture " << READ_CAPTURE(move)<<endl;
	//cout << "type " << READ_MOVE_TYPE(move)<<endl;
	//cout << "color " << READ_COLOR(move)<<endl;
	//cout << "id " << READ_LISTID(move)<<endl;

	//game.setFEN("rnbqkb1r/ppp1pppp/3N1n2/3p4/8/8/PPPPPPPP/R1BQKBNR/");
	////game.setFEN("r6r/1p3ppp/p5k1/2P2b2/8/3P2N1/P2Q1PqP/2R1K2R/");
	//game.setIsKingMoved(WHITE, true);
	//game.setIsKingMoved(BLACK, true);
	////game.setIsRshRookMoved(WHITE, true);
	//game.setIsRshRookMoved(BLACK, true);
	//game.setIsLshRookMoved(WHITE, true);
	//game.setIsLshRookMoved(BLACK, true);

	
	//cout << game.getMoves(BLACK);

	//p2.alphaBeta(4, -INF, INF);
	//cout << game.getMoves(BLACK);

	//int move = CreateListItem(50, 58, PAWN, 0, 8, 0);
	//game.makeMove(move);

	//ShowBoardVector(game.getFigureFromCoord(), WHITE);
	//ShowBits(game.getFigureBoard(QUEEN, WHITE));
	//(move);
	/*while (true)
	{

		p1.alphaBeta(5, -INF, INF);
		UI(game, BLACK);

	}*/
	//while (true)
	//{
	//	UI(game, WHITE);
	//	p2.alphaBeta(5, -INF, INF);
	//}

	//ShowBoardVector(game.getFigureFromCoord(), WHITE);
	//for (int i = PAWN; i <= KING; ++i)
	//	ShowBits(game.getFigureBoard(i, WHITE));
	//cout << "black: " << endl;
	//for (int i = PAWN; i <= KING; ++i)
	//	ShowBits(game.getFigureBoard(i, BLACK));
	//ShowBoardVector(game.getFigureFromCoord(), BLACK);

	for (int i = 0; i < 40; ++i)
	{
		p1.alphaBeta(6, -INF, INF);
		p2.alphaBeta(6, -INF, INF);
	}

	cout << game.getPGN();
}

