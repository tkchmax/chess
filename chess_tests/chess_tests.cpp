#include "pch.h"
#include "CppUnitTest.h"
#include "../chess_oop/main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

RawMoves GenerateMoves_tests(const Position& position, shared_ptr<Figure> figure)
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

//int ToSquareNumber(string coord)
//{
//	char file = coord[0];
//	char rank = coord[1];
//	int x = int(file % 97);
//	int y = int(rank % 49) * 8;
//	return x + y;
//}

namespace figuretests
{
	TEST_CLASS(PAWN_TESTS)
	{
		TEST_METHOD(GENERATE_PAWN_MOVES_START_POSITION)
		{
			_initializeRays();
			Position start_position;

			shared_ptr<Figure> white_pawn = start_position.getFigure(PAWN, WHITE);
			shared_ptr<Figure> black_pawn = start_position.getFigure(PAWN, BLACK);

			RawMoves white_moves = GenerateMoves_tests(start_position, white_pawn);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_pawn);

			const U64 EXPECTED_WHITE_SILENT_MOVES = 0xFFFF0000;
			const U64 EXPECTED_BLACK_SILENT_MOVES = 0xFFFF00000000;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_WHITE_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_BLACK_SILENT_MOVES);

			Assert::AreEqual(white_moves.takes, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.takes, EXPECTED_CAPTURE_MOVES);

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

			RawMoves white_moves = GenerateMoves_tests(start_position, white_knight);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_knight);

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
			
			moves = GenerateMoves_tests(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White knight on e4
			//Blockers: f6, g5, g3, f2, d2, c3, c5, d6
			position.setFEN("8/8/3B1R2/2Q3R1/4N3/2P3R1/3P1R2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;
			
			moves = GenerateMoves_tests(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//White knight on e4
			//Blockers: d5, e5, f5, f4, f3, e3, d3, d4
			//Captures: f6, g5, g3, f2, d2, c3, c5, d6
			position.setFEN("8/8/3n1b2/2qRBPp1/3QNB2/2pKPQr1/3n1b2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x284400442800;

			moves = GenerateMoves_tests(position, knight);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			//Knight on e4
			//Blockers: d6, f6
			//Captures: g5, g3, f2, d2, c3, c5
			position.setFEN("8/8/3R1P2/2q3p1/4N3/2q3p1/3b1b2/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x4400442800;
			
			moves = GenerateMoves_tests(position, knight);
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

			RawMoves white_moves = GenerateMoves_tests(start_position, white_bishop);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_bishop);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.takes, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.takes, EXPECTED_CAPTURE_MOVES);
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

			RawMoves white_moves = GenerateMoves_tests(start_position, start_position.getFigure(ROOK, WHITE));
			RawMoves black_moves = GenerateMoves_tests(start_position, start_position.getFigure(ROOK, BLACK));

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

			moves = GenerateMoves_tests(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);


			//	//White rook is on e4
			//	//Captures: e5, d4, f4, e3
			//	//Blockers: e6, h4, g4
			position.setFEN("8/8/4P3/4n3/3qRpNB/4p3/8/8/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0x1028100000;

			moves = GenerateMoves_tests(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);
		
			////-------CAPTURE MOVES-------
			////Rook is on e4
			////Captures: e6, h4, g4
			////Blockers: NO
			position.setFEN("8/8/4p3/8/4R1qr/8/8/8/");
			EXPECTED_SILENT_MOVES = 0x102F101010;
			EXPECTED_CAPTURE_MOVES = 0x100040000000;

			moves = GenerateMoves_tests(position, rook);
			Assert::AreEqual(moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(moves.takes, EXPECTED_CAPTURE_MOVES);

			////Rook is on e4
			////Blockers: e5, d4, f4, e3;
			////Captures: e6,g4,e1,b4;
			position.setFEN("8/8/4p3/4B3/1n1PRNqn/4Q3/8/4q3/");
			EXPECTED_SILENT_MOVES = 0;
			EXPECTED_CAPTURE_MOVES = 0;

			moves = GenerateMoves_tests(position, rook);
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

			RawMoves white_moves = GenerateMoves_tests(start_position, white_queen);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_queen);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
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

			RawMoves white_moves = GenerateMoves_tests(start_position, white_king);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_king);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
		}
	};
}



