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

	bool isListsEqual(deque<int> list1, deque<int> list2)
	{
		if (list1.size() != list2.size())
			return false;

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

		TEST_METHOD(GENERATE_PAWN_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("rnb1k1nr/ppp2ppp/8/3ppq2/2PP1BP1/b7/PP2PP1P/RN1QKBNR/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(PAWN, WHITE);
			MoveList blackList = position.getFigureMoveList(PAWN, BLACK);

			// white pawns
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b2"), to_square("b3"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b2"), to_square("b4"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b2"), to_square("a3"), PAWN, BISHOP, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c4"), to_square("c5"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c4"), to_square("d5"), PAWN, PAWN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d4"), to_square("e5"), PAWN, PAWN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e2"), to_square("e3"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e2"), to_square("e4"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f2"), to_square("f3"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("g4"), to_square("g5"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("g4"), to_square("f5"), PAWN, QUEEN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("h2"), to_square("h3"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("h2"), to_square("h4"), PAWN, 0, MOVE_TYPE_SILENT, WHITE);

			//black pawns 
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a7"), to_square("a6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a7"), to_square("a5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("b7"), to_square("b6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("b7"), to_square("b5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("c7"), to_square("c6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("c7"), to_square("c5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("d5"), to_square("c4"), PAWN, PAWN, MOVE_TYPE_TAKE, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e5"), to_square("d4"), PAWN, PAWN, MOVE_TYPE_TAKE, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e5"), to_square("f4"), PAWN, BISHOP, MOVE_TYPE_TAKE, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e5"), to_square("e4"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f7"), to_square("f6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("g7"), to_square("g6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("g7"), to_square("g5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h7"), to_square("h6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h7"), to_square("h5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
		}
	};

	TEST_CLASS(KNGIHT_TESTS)
	{
		TEST_METHOD(GENERATE_KNIGHT_MOVES_START_POSITION)
		{
			_initializeRays();
			Position start_position;

			shared_ptr<Figure> white_knight = start_position.getFigure(KNIGHT, WHITE);
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

		TEST_METHOD(GENERATE_KNGIHT_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("rnb1k1nr/ppp2ppp/8/3ppq2/2PP1BP1/b7/PP2PP1P/RN1QKBNR/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(KNIGHT, WHITE);
			MoveList blackList = position.getFigureMoveList(KNIGHT, BLACK);
			//white knights
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b1"), to_square("a3"), KNIGHT, BISHOP, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b1"), to_square("c3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("b1"), to_square("d2"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("g1"), to_square("f3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("g1"), to_square("h3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE);

			//black knights
			BLACK_EXPECTED_LIST += CreateListItem(to_square("b8"), to_square("a6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("b8"), to_square("c6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("b8"), to_square("d7"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("g8"), to_square("e7"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("g8"), to_square("f6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("g8"), to_square("h6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
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

		TEST_METHOD(GENERATE_BISHOP_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("rnb1k1nr/ppp2ppp/8/3ppq2/2PP1BP1/b7/PP2PP1P/RN1QKBNR/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(BISHOP, WHITE);
			MoveList blackList = position.getFigureMoveList(BISHOP, BLACK);
			//white bishop
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f1"), to_square("g2"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f1"), to_square("h3"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("e5"), BISHOP, PAWN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("e3"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("d2"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("c1"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("g5"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("h6"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("f4"), to_square("g3"), BISHOP, 0, MOVE_TYPE_SILENT, WHITE);

			//black bishops
			BLACK_EXPECTED_LIST += CreateListItem(to_square("c8"), to_square("d7"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("c8"), to_square("e6"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("b4"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("c5"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("d6"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("e7"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("f8"), BISHOP, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a3"), to_square("b2"), BISHOP, PAWN, MOVE_TYPE_TAKE, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
		}
	};

	TEST_CLASS(ROOK_TESTS)
	{
		TEST_METHOD(GENERATE_ROOK_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_rook = start_position.getFigure(ROOK, WHITE);
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

		TEST_METHOD(GENERATE_ROOK_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("r1b1k2r/1pp2pp1/8/2Rppq2/2P2BP1/b6N/PP2PP1P/R3KB2/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(ROOK, WHITE);
			MoveList blackList = position.getFigureMoveList(ROOK, BLACK);

			//white rooks
			WHITE_EXPECTED_LIST += CreateListItem(to_square("a1"), to_square("b1"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("a1"), to_square("c1"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("a1"), to_square("d1"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c5"), to_square("c6"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c5"), to_square("b5"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c5"), to_square("a5"), ROOK, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c5"), to_square("c7"), ROOK, PAWN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("c5"), to_square("d5"), ROOK, PAWN, MOVE_TYPE_TAKE, WHITE);
			//black rooks
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a8"), to_square("a7"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a8"), to_square("a6"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a8"), to_square("a5"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a8"), to_square("a4"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("a8"), to_square("b8"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("h7"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("h6"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("h5"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("h4"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("g8"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("f8"), ROOK, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("h8"), to_square("h3"), ROOK, KNIGHT, MOVE_TYPE_TAKE, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(blackList.capture, BLACK_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
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

		TEST_METHOD(GENERATE_QUEEN_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("rnb1k1nr/ppp2ppp/8/3ppq2/2PP1BP1/b7/PP2PP1P/RN1QKBNR/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(QUEEN, WHITE);
			MoveList blackList = position.getFigureMoveList(QUEEN, BLACK);
			//white queen
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("c1"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("c2"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("b3"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("a4"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("d2"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("d1"), to_square("d3"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE);

			//black queen
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("e6"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("d7"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("g5"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("h5"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("g6"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("f6"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("e4"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("d3"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("c2"), QUEEN, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("b1"), QUEEN, KNIGHT, MOVE_TYPE_TAKE, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("f4"), QUEEN, BISHOP, MOVE_TYPE_TAKE, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("f5"), to_square("g4"), QUEEN, PAWN, MOVE_TYPE_TAKE, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();
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

		TEST_METHOD(GENERATE_KING_MOVELIST)
		{
			_initializeRays();
			Position position;
			position.setFEN("r1b1k2r/1pp2Np1/8/2Rppq2/2P2BP1/b6N/PP2qp1P/R3KB2/");

			MoveList WHITE_EXPECTED_LIST, BLACK_EXPECTED_LIST;
			MoveList whiteList = position.getFigureMoveList(KING, WHITE);
			MoveList blackList = position.getFigureMoveList(KING, BLACK);

			//white king
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e1"), to_square("d1"), KING, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e1"), to_square("d2"), KING, 0, MOVE_TYPE_SILENT, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e1"), to_square("e2"), KING, QUEEN, MOVE_TYPE_TAKE, WHITE);
			WHITE_EXPECTED_LIST += CreateListItem(to_square("e1"), to_square("f2"), KING, PAWN, MOVE_TYPE_TAKE, WHITE);

			//black king
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("d8"), KING, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("d7"), KING, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("e7"), KING, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("f8"), KING, 0, MOVE_TYPE_SILENT, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("g8"), KING, 0, MOVE_TYPE_0_0, BLACK);
			BLACK_EXPECTED_LIST += CreateListItem(to_square("e8"), to_square("f7"), KING, KNIGHT, MOVE_TYPE_TAKE, BLACK);

			bool isWhiteSilentListsEqual = isListsEqual(whiteList.silent, WHITE_EXPECTED_LIST.silent);
			bool isWhiteCaptureListsEqual = isListsEqual(whiteList.capture, WHITE_EXPECTED_LIST.capture);
			if (!isWhiteSilentListsEqual || !isWhiteCaptureListsEqual)
				Assert::Fail();

		}

		TEST_METHOD(SHORTCASTLING_TEST)
		{
			_initializeRays();
			Game start_game;
			Game game;
			game.setFEN("rnbqkbnr/pppppppp/8/4Q3/2B5/R1NBPN2/PPPP1PPP/4K2R/");

			bool isSCastlingPossible_startPosition = start_game.isCastlingPossible(WHITE, MOVE_TYPE_0_0);
			bool isSCastlingPossible_position = game.isCastlingPossible(WHITE, MOVE_TYPE_0_0);
			Assert::AreEqual(false, isSCastlingPossible_startPosition);
			Assert::AreEqual(true, isSCastlingPossible_position);

			bool isLCastlingPossible_startPosition = start_game.isCastlingPossible(WHITE, MOVE_TYPE_0_0_0);
			bool isLCastlingPossible_position = game.isCastlingPossible(WHITE, MOVE_TYPE_0_0_0);
			Assert::AreEqual(false, isLCastlingPossible_startPosition);
			Assert::AreEqual(true, isLCastlingPossible_position);

			int shortCastling = CreateListItem(4, 6, KING, 0, MOVE_TYPE_0_0, WHITE);
			game.makeMove(shortCastling);
			string EXPECTED_SHORTCASTLING_FEN = "rnbqkbnr/pppppppp/8/4Q3/2B5/R1NBPN2/PPPP1PPP/5RK1/";
			Assert::AreEqual(EXPECTED_SHORTCASTLING_FEN, game.getFEN());
			game.undoMove(shortCastling);

			int longCastling = CreateListItem(4, 2, KING, 0, MOVE_TYPE_0_0_0, WHITE);
			game.makeMove(longCastling);
			string EXPECTED_LONGCASTLING_FEN = "rnbqkbnr/pppppppp/8/4Q3/2B5/R1NBPN2/PPPP1PPP/2KR3R/";
			Assert::AreEqual(EXPECTED_LONGCASTLING_FEN, game.getFEN());

			




		}
	};

	TEST_CLASS(MOVE_TESTS)
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

		TEST_METHOD(MAKEnUNDO_MOVE_TEST)
		{
			Game game;
			vector<vector<int>> s_FigureFromCoord = game.getFigureFromCoord();
			string s_fen = game.getFEN();

			vector<int> moves;
			moves.push_back(CreateListItem(to_square("a2"), to_square("a4"), PAWN, 0, MOVE_TYPE_SILENT, WHITE));
			moves.push_back(CreateListItem(to_square("e2"), to_square("e4"), PAWN, 0, MOVE_TYPE_SILENT, WHITE));
			moves.push_back(CreateListItem(to_square("b1"), to_square("c3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE));
			moves.push_back(CreateListItem(to_square("g1"), to_square("h3"), KNIGHT, 0, MOVE_TYPE_SILENT, WHITE));
			moves.push_back(CreateListItem(to_square("d1"), to_square("h5"), QUEEN, 0, MOVE_TYPE_SILENT, WHITE));
			moves.push_back(CreateListItem(to_square("h5"), to_square("h7"), QUEEN, PAWN, MOVE_TYPE_TAKE, WHITE));
			moves.push_back(CreateListItem(to_square("e7"), to_square("e5"), PAWN, 0, MOVE_TYPE_SILENT, BLACK));
			moves.push_back(CreateListItem(to_square("e8"), to_square("e7"), KING, 0, MOVE_TYPE_SILENT, BLACK));
			moves.push_back(CreateListItem(to_square("a7"), to_square("a6"), PAWN, 0, MOVE_TYPE_SILENT, BLACK));
			moves.push_back(CreateListItem(to_square("a8"), to_square("a7"), ROOK, 0, MOVE_TYPE_SILENT, BLACK));
			moves.push_back(CreateListItem(to_square("g8"), to_square("f6"), KNIGHT, 0, MOVE_TYPE_SILENT, BLACK));
			moves.push_back(CreateListItem(to_square("h8"), to_square("g8"), ROOK, 0, MOVE_TYPE_SILENT, BLACK));

			for (auto move : moves)
				game.makeMove(move);

			string EXPECTED_FEN = "1nbq1br1/rpppkppQ/p4n2/4p3/P3P3/2N4N/1PPP1PPP/R1B1KB1R/";

			Assert::AreEqual(EXPECTED_FEN, game.getFEN());

			reverse(moves.begin(), moves.end());

			for (auto move : moves)
				game.undoMove(move);


			Assert::AreEqual(s_fen, game.getFEN());
			vector<vector<int>> figureFromCoord = game.getFigureFromCoord();

			for (int i = 0; i < 2; ++i)
				for (int j = 0; j < 64; ++j)
					Assert::AreEqual(s_FigureFromCoord[i][j], figureFromCoord[i][j]);
		}
	};
}

