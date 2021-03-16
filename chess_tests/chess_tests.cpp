#include "pch.h"
#include "CppUnitTest.h"
#include "../chess_oop/main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

int to_square(string coord)
{
	char file = coord[0];
	char rank = coord[1];
	int x = int(file % 97);
	int y = int(rank % 49) * 8;
	return x + y;
}
//int ToSquareNumber(string coord)
//{
//	char file = coord[0];
//	char rank = coord[1];
//	int x = int(file % 97);
//	int y = int(rank % 49) * 8;
//	return x + y;
//}

namespace tests
{
	int to_square(string coord)
	{
		char file = coord[0];
		char rank = coord[1];
		int x = int(file % 97);
		int y = int(rank % 49) * 8;
		return x + y;
	}

	RawMoves GenerateMoves(const Position& position, shared_ptr<Figure> figure)
	{
		int color = figure->getColor();
		int oppositeColor = (color == WHITE) ? BLACK : WHITE;

		U64 blockers = position.getSideBoard(color);
		U64 opposite = position.getSideBoard(oppositeColor);

		vector<int> figure_squares = figure->getPiecesSquares();

		RawMoves moves;
		for (int i = 0; i < figure_squares.size(); ++i)
			moves += figure->getMoveBoards(figure_squares[i], blockers, opposite);
		return moves;

	}


	TEST_CLASS(PAWN_TESTS)
	{
		TEST_METHOD(GENERATE_PAWN_MOVES_START_POSITION)
		{
			_initializeRays();
			Position start_position;

			shared_ptr<Figure> white_pawn = start_position.getFigure(PAWN, WHITE);
			shared_ptr<Figure> black_pawn = start_position.getFigure(PAWN, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, white_pawn);
			RawMoves black_moves = GenerateMoves(start_position, black_pawn);

			const U64 EXPECTED_WHITE_SILENT_MOVES = 0xFFFF0000;
			const U64 EXPECTED_BLACK_SILENT_MOVES = 0xFFFF00000000;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_WHITE_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_BLACK_SILENT_MOVES);

			Assert::AreEqual(white_moves.takes, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.takes, EXPECTED_CAPTURE_MOVES);

		}

		TEST_METHOD(GENERATE_WHITE_PAWN_MOVES)
		{
			_initializeRays();
			Position position;
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			shared_ptr<Figure> pawns = position.getFigure(PAWN, WHITE);

			//White pawns on start position
			//Blockers: a3, c3, e4, f3, h3
			position.setFEN("8/8/8/8/4Q3/B1B2N1N/PPPPPPPP/8/");
			EXPECTED_SILENT_MOVES = 0x4A5A0000;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White pawns on start position
			//Blockers: a3, c3, e4, f3, h3
			//Captures: b3, e3, g3
			position.setFEN("8/8/8/8/4Q3/BqB1nNkN/PPPPPPPP/8/");
			EXPECTED_SILENT_MOVES = 0x8080000;
			EXPECTED_CAPTURE_MOVES = 0x520000;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White pawns on start position
			//Blockers: a3, c3, e4, f3, h3
			//Captures: a-h(3)
			position.setFEN("8/8/8/8/8/nqnnnnkn/PPPPPPPP/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0xFF0000;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White pawns on a-h(2), a5, c5, f5, g5
			//Blockers: NO
			//Captures: a6, d6, h6
			position.setFEN("8/8/p2p3p/P1P2PP1/8/8/PPPPPPPP/8/");
			EXPECTED_SILENT_MOVES = 0x6400FFFF0000;
			EXPECTED_CAPTURE_MOVES = 0x880000000000;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

		}

		TEST_METHOD(GENERATE_BLACK_PAWN_MOVES)
		{
			_initializeRays();
			Position position;
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			shared_ptr<Figure> pawns = position.getFigure(PAWN, BLACK);

			//Black pawns on start position
			//Blockers: a6, e6, g6
			//Captures: NO
			position.setFEN("8/pppppppp/r3n1n1/8/8/8/8/8/");
			EXPECTED_SILENT_MOVES = 0xAEAE00000000;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);


			//Black pawns on start position
			//Blockers: a6, e6, g6
			//Captures: 
			position.setFEN("8/pppppppp/r1PBn1nN/8/8/8/8/8/");
			EXPECTED_SILENT_MOVES = 0x222200000000;
			EXPECTED_CAPTURE_MOVES = 0x8C0000000000;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			
			//Black pawns on a-h(7), c4, f4, h4
			//Blockers: c3
			//Captures: e3, g3
			position.setFEN("8/pppppppp/8/8/2p2p1p/2B1B1B1/8/8/");
			EXPECTED_SILENT_MOVES = 0xFFFF00A00000;
			EXPECTED_CAPTURE_MOVES = 0x500000;

			moves = GenerateMoves(position, pawns);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
		}
	};

	TEST_CLASS(KNGIHT_TESTS)
	{
		TEST_METHOD(GENERATE_KNIGHT_MOVES_START_POSITION)
		{
			_initializeRays();
			Position start_position;

			shared_ptr<Figure> white_knight = start_position.getFigure(KNIGHT,WHITE);
			shared_ptr<Figure> black_knight = start_position.getFigure(KNIGHT, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, white_knight);
			RawMoves black_moves = GenerateMoves(start_position, black_knight);

			const U64 EXPECTED_WHITE_SILENT_MOVES = 0x50000 | 0xA00000; //lsh knight | rsh knight
			const U64 EXPECTED_BLACK_SILENT_MOVES = 0x50000000000 | 0xA00000000000;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_WHITE_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_BLACK_SILENT_MOVES);

			Assert::AreEqual(white_moves.takes, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.takes, EXPECTED_CAPTURE_MOVES);
		}

		TEST_METHOD(GENERATE_KNGIHT_MOVES)
		{
			Position position;
			shared_ptr<Figure> knight = position.getFigure(KNIGHT, WHITE);
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			//White knight on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4
			position.setFEN("8/8/8/3BRB2/3PNP2/3KQR2/8/8/");
			EXPECTED_SILENT_MOVES = 0x284400442800;
			EXPECTED_CAPTURE_MOVES = 0;
			
			moves = GenerateMoves(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White knight on e4
			//Blockers: f6, g5, g3, f2, d2, c3, c5, d6
			position.setFEN("8/8/3B1R2/2Q3R1/4N3/2P3R1/3P1R2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;
			
			moves = GenerateMoves(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White knight on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4
			//Captures: f6, g5, g3, f2, d2, c3, c5, d6
			position.setFEN("8/8/3n1b2/2qRBPp1/3QNB2/2pKPQr1/3n1b2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x284400442800;

			moves = GenerateMoves(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//Knight on e4
			//Blockers: d6, f6
			//Captures: g5, g3, f2, d2, c3, c5
			position.setFEN("8/8/3R1P2/2q3p1/4N3/2q3p1/3b1b2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x4400442800;
			
			moves = GenerateMoves(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

		}
	};

	TEST_CLASS(BISHOP_TESTS)
	{
		TEST_METHOD(GENERATE_BISHOP_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_bishop = start_position.getFigure(BISHOP, WHITE);
			shared_ptr<Figure> black_bishop = start_position.getFigure(BISHOP, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, white_bishop);
			RawMoves black_moves = GenerateMoves(start_position, black_bishop);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.takes, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.takes, EXPECTED_CAPTURE_MOVES);
		}

		TEST_METHOD(GENERATE_BISHOP_MOVES)
		{
			_initializeRays();

			Position position;
			shared_ptr<Figure> bishop = position.getFigure(BISHOP, WHITE);
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			//White bishop on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4 
			position.setFEN("8/8/8/3RNP2/3QBK2/3NPP2/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, bishop);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White bishop on e4 
			//Blockers: e5, f4, e3, d4 
			//Captures: d5, f5, f3, d3
			position.setFEN("8/8/8/3nNp2/3QBK2/3bPk2/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x2800280000;

			moves = GenerateMoves(position, bishop);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White bishop on e4
			//Blockers: e8, f3, b1
			//Captures: a8
			position.setFEN("p3N3/6p1/7p/8/4Bp2/5N2/6p1/1K6/");
			EXPECTED_SILENT_MOVES = 0x82442800080400;
			EXPECTED_CAPTURE_MOVES = 0x100000000000000;

			moves = GenerateMoves(position, bishop);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White bishop on e4
			//Blockers: d3
			//Captures: f5, d5, h1
			position.setFEN("8/8/2P3p1/3n1p2/4B3/3Q4/2n5/7q/");
			EXPECTED_SILENT_MOVES = 0x204000;
			EXPECTED_CAPTURE_MOVES = 0x2800000080;

			moves = GenerateMoves(position, bishop);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
		}
	};

	TEST_CLASS(ROOK_TESTS)
	{
		TEST_METHOD(GENERATE_ROOK_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_rook = start_position.getFigure(ROOK,WHITE);
			shared_ptr<Figure> black_rook = start_position.getFigure(ROOK, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, start_position.getFigure(ROOK, WHITE));
			RawMoves black_moves = GenerateMoves(start_position, start_position.getFigure(ROOK, BLACK));

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
		}
		TEST_METHOD(GENERATE_ROOK_MOVES)
		{
			_initializeRays();
			Position position;
			shared_ptr<Figure> rook = position.getFigure(ROOK, WHITE);
			RawMoves moves;
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			//-------SILENT MOVES-------
			//White rook is on e4
			//Blockers: e6, h4, g4
			//Captures: NO
			position.setFEN("8/8/4P3/8/4R1PP/8/8/8/");
			EXPECTED_SILENT_MOVES = 0x102F101010;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);


			//	//White rook is on e4
			//	//Captures: e5, d4, f4, e3
			//	//Blockers: e6, h4, g4
			position.setFEN("8/8/4P3/4n3/3qRpNB/4p3/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x1028100000;

			moves = GenerateMoves(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
		
			////Rook is on e4
			////Captures: e6, h4, g4
			////Blockers: NO
			position.setFEN("8/8/4p3/8/4R1qr/8/8/8/");
			EXPECTED_SILENT_MOVES = 0x102F101010;
			EXPECTED_CAPTURE_MOVES = 0x100040000000;

			moves = GenerateMoves(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			////Rook is on e4
			////Blockers: e5, d4, f4, e3;
			////Captures: e6,g4,e1,b4;
			position.setFEN("8/8/4p3/4B3/1n1PRNqn/4Q3/8/4q3/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
		}
	};

	TEST_CLASS(QUEEN_TESTS)
	{
		TEST_METHOD(GENERATE_QUEEN_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_queen = start_position.getFigure(QUEEN, WHITE);
			shared_ptr<Figure> black_queen = start_position.getFigure(QUEEN, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, white_queen);
			RawMoves black_moves = GenerateMoves(start_position, black_queen);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
		}
		
		TEST_METHOD(GENERATE_QUEEN_MOVES)
		{
			_initializeRays();

			Position position;
			shared_ptr<Figure> queen = position.getFigure(QUEEN, WHITE);
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			//White queen on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4 
			position.setFEN("8/8/8/3RNP2/3BQK2/3NPP2/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White queen on e4 
			//Blockers: e5, f4, e3, d4 
			//Captures: d5, f5, f3, d3
			position.setFEN("8/8/8/3nNp2/3PQK2/3bPk2/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x2800280000;

			moves = GenerateMoves(position, queen);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//Rook is on e4
			//Captures: e6, h4, g4
			//Blockers: NO
			position.setFEN("b7/7k/4p1N1/8/4Q1qr/8/8/8/");
			EXPECTED_SILENT_MOVES = 0x204382F385492;
			EXPECTED_CAPTURE_MOVES = 0x100100040000000;

			moves = GenerateMoves(position, queen);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
	

		}
	};

	TEST_CLASS(KING_TESTS)
	{
		TEST_METHOD(GENERATE_KING_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_king = start_position.getFigure(KING, WHITE);
			shared_ptr<Figure> black_king = start_position.getFigure(KING, BLACK);

			RawMoves white_moves = GenerateMoves(start_position, white_king);
			RawMoves black_moves = GenerateMoves(start_position, black_king);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
		}

		TEST_METHOD(GENERATE_KING_MOVES)
		{
			_initializeRays();

			Position position;
			shared_ptr<Figure> king = position.getFigure(KING, WHITE);
			U64 EXPECTED_SILENT_MOVES;
			U64 EXPECTED_CAPTURE_MOVES;
			RawMoves moves;

			//White king on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4 
			position.setFEN("8/8/8/3PBR2/3PKP2/3PPB2/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves(position, king);

			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White king on e4
			//Blockers: f3
			//Captures: f4, d5
			position.setFEN("8/8/8/3n4/4Kn2/5B2/8/8/");
			EXPECTED_SILENT_MOVES = 0x3008180000;
			EXPECTED_CAPTURE_MOVES = 0x820000000;

			moves = GenerateMoves(position, king);

			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

		}
	};
}

bool isListsEqual(deque<int> list1, deque<int> list2)
{
	int size = list1.size();
	for (int i = 0; i < size; ++i)
	{
		int silent = list1[i];
		for (int j = 0; j <= size; ++j)
		{
			if (j == size)
				return false;
			if (silent == list2[j])
				break;
		}
	}
	return true;
}

	TEST_CLASS(GENERATE_MOVE_LIST)
	{
		TEST_METHOD(GENERATE_MOVE_LIST_START_POSITION)
		{
			_initializeRays();
			
			Position start_position;
			MoveList whiteList = start_position.getMoves(WHITE);
			MoveList blackList = start_position.getMoves(BLACK);


			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			//white pawns 
			for (int i = 8; i < 16; ++i)
			{
				WHITE_EXPECTED_LIST += CreateListItem(i, i + 8, PAWN, 0, MOVE_TYPE_SILENT, WHITE);
				WHITE_EXPECTED_LIST += CreateListItem(i, i + 16, PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			}
			//black pawns
			for (int i = 55; i >= 48; --i)
			{
				BLACK_EXPECTED_LIST += CreateListItem(i, i - 8, PAWN, 0, MOVE_TYPE_SILENT, BLACK);
				BLACK_EXPECTED_LIST += CreateListItem(i, i - 16, PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			}

			int b1 = to_square("b1");
			int g1 = to_square("g1");
			WHITE_EXPECTED_LIST += CreateListItem(b1, to_square("a3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(b1, to_square("c3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(g1, to_square("f3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(g1, to_square("h3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);

			int b8 = to_square("b8");
			int g8 = to_square("g8");
			BLACK_EXPECTED_LIST += CreateListItem(b8, to_square("a6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(b8, to_square("c6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(g8, to_square("f6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(g8, to_square("h6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);


			Assert::AreEqual(blackList.size(), BLACK_EXPECTED_LIST.size());
			Assert::AreEqual(whiteList.size(), WHITE_EXPECTED_LIST.size());
			//white moves
			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
			//black moves;
			bool isBlackListsEqual = isListsEqual(blackList.silent, BLACK_EXPECTED_LIST.silent);
			bool isBlackCaptureListsEqual = isListsEqual(blackList.capture, BLACK_EXPECTED_LIST.capture);
			if (!isBlackListsEqual || !isBlackCaptureListsEqual)
				Assert::Fail();


		}
	};

