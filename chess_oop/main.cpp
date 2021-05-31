#include "main.h"
#include <iomanip>

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


struct Handler
{
	virtual int handle(vector<string>::iterator& parameters) = 0;
	virtual ~Handler() {}
};
struct NewGameHandler : Handler
{
	string getAvailibleMoves(const Game& game, int color)
	{
		string res = "";
		MoveList list = game.getMoves(color);
		int move = 0, prev_move = 0;
		do
		{
			prev_move = move;
			move = list.next();
			if (READ_FIGURE(move) != READ_FIGURE(prev_move))
				res += '\n';
			if (move != 0)
				res += game.getNotationOfMove(move) + "  ";
		} while (move);
		return res;
	}
	int UI(const Game& game, int color)
	{
		string s_move;
		//system("CLS");
		cout << "\nhistory:\n" << game.getPGN() << endl;
		//printf("\033[32mAvailible moves\n%s\033[0m\n\n", getAvailibleMoves(game, BLACK).c_str());

		cout << "your turn >> "; cin >> s_move;
		cin.ignore();

		if (s_move == "exit")
			return EXIT_HANDLE;
		if (s_move == "undo")
			return UNDO_HANDLE;

		MoveList availibleMoves = game.getMoves(color);

		int move;
		while (move = availibleMoves.next())
		{
			if (game.getNotationOfMove(move) == s_move)
				return move;
		}

		string err_message = "ERROR: There is no move " + s_move + " in current position";
		throw(err_message);
	}
	int handle(vector<string>::iterator& parameters) override
	{
		string s_color = *(++parameters);
		int depth = stoi(*(++parameters));

		if (s_color != "w" && s_color != "b")
			throw "ERROR: invalid color parameter";

		int color = (s_color == "w") ? WHITE : BLACK;

		Game game;
		//game.setFEN("r4rk1/1pp1q2p/p1n1bpp1/3Pp3/2P1N3/P3bB1P/2R3PB/3Q1R1K/");
		//game.setIsKingMoved(WHITE, true);
		//game.setIsKingMoved(BLACK, true);
		//game.setIsRshRookMoved(WHITE, true);
		//game.setIsRshRookMoved(BLACK, true);
		//game.setIsLshRookMoved(WHITE, true);
		//game.setIsLshRookMoved(BLACK, true);

		int opColor = (color == WHITE) ? BLACK : WHITE;
		Player computer(&game, opColor);

		int move;
		function<int()> white_turn = [this, &game, &computer, depth, color, &move] {
			move = UI(game, color);
			if (move < 0) return move;
			game.makeMove(move);
			game.saveMove(move);
			game.saveNotationOfMove(move);
			cout << "\ncomputing...\n\n";
			computer.alphaBeta(depth, -INF, INF);
			return 0;
		};
		function<int()> black_turn = [this, &game, &computer, depth,color, &move] {
			cout << "\ncomputing...\n\n";
			computer.alphaBeta(depth, -INF, INF);
			move = UI(game, color);
			if (move < 0) return move;
			game.makeMove(move);
			game.saveMove(move);
			game.saveNotationOfMove(move);
			return 0;
		};
		function<int()> play = (color == WHITE) ? white_turn : black_turn;

		while (!game.isGameOver())
		{
			try {
				switch (play())
				{
				case EXIT_HANDLE:
					return 0;
				case UNDO_HANDLE:
					for (int i = 0; i < 2; ++i)
					{
						game.undoMove(game.removeLastMove());
						string s = game.removeLastNotation();
						cout << "removed " << s << endl;
					}
					if (color == BLACK)
						play = white_turn;
					break;
				default:
					break;
				}
			}
			catch (string err)
			{
				printf("\033[31m%s\033[0m\n", err.c_str());
				printf("\033[32mAvailible moves\n%s\033[0m\n", getAvailibleMoves(game, color).c_str());
				if (color == BLACK)
					play = white_turn;
			}
		}
		return 0;
	}
private:
	int last_move;
	vector<int> moves;
};
struct ExitHandler : Handler
{
	int handle(vector<string>::iterator& parameters)
	{
		return EXIT_HANDLE;
	}
};
struct ClsHandler : Handler
{
	int handle(vector<string>::iterator&)
	{
		system("CLS");
		return 0;
	}
};
struct EvalHandler : Handler
{
	int handle(vector<string>::iterator& parameters) override
	{
		string fen = *(++parameters) + '/';
		Game game;
		game.setFEN(fen);
		//cout << "--------------------------------------\n"
		//	<<"Total Material: " << game.getMaterialEval(WHITE) << " |\n"
		//	<< "Total Strategy: " << game.getStrategyEval(WHITE) << " |\n"
		//	<< "Total Mobility: " << game.getMobilityEval(WHITE) << " |\n"
		//	<< "Doubled pawns: " << game.getDoubledPawnEval(WHITE) << " |\n"
			//<< "--------------------------------------------------\n"
		cout << "Total evaluation: " << (double)game.evaluate(WHITE) / 100 << " (white side)\n";
		//<< "-------------------------------------------\n";
		return 0;
	}
};

void UI()
{
	map<string, Handler*> handlers;
	handlers["newgame"] = new NewGameHandler();
	handlers["cls"] = new ClsHandler();
	handlers["eval"] = new EvalHandler();
	handlers["exit"] = new ExitHandler();
	handlers["q"] = new ExitHandler();

	string raw_command;
	int ret = 0;
	while (ret != EXIT_HANDLE)
	{
		cout << ">> ";
		getline(cin, raw_command);
		if (raw_command == "")
			continue;
		stringstream ss(raw_command);

		vector<string> parameters;
		do
		{
			parameters.push_back(string());
		} while (ss >> parameters.back());

		vector<string>::iterator param = parameters.begin();
		try {
			ret = handlers.at(parameters[0])->handle(param);
		}
		catch (std::out_of_range & ex)
		{
			printf("\033[31mERROR: command not found\033[0m\n");
		}
		catch (std::invalid_argument & ex)
		{
			printf("\033[31mERROR: invalid parameters\033[0m\n");
		}

	}
}


int main()
{
	_initializeRays();
	UI();

	Game game;
	Player p1(&game, WHITE);
	Player p2(&game, BLACK);

	//ShowBoardVector(game.getFigureFromCoord(), WHITE);
	//for (int i = PAWN; i <= KING; ++i)
	//	ShowBits(game.getFigureBoard(i, WHITE));
	//cout << "black: " << endl;
	//for (int i = PAWN; i <= KING; ++i)
	//	ShowBits(game.getFigureBoard(i, BLACK));
	//ShowBoardVector(game.getFigureFromCoord(), BLACK);

	//for (int i = 0; i < 40; ++i)
	//{
	//	p1.alphaBeta(5, -INF, INF);
	//	p2.alphaBeta(5, -INF, INF);
	//}

	cout << game.getPGN();
}

