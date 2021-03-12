#include "pch.h"
#include "CppUnitTest.h"
#include "../chess_oop/main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

RawMoves GenerateMoves_tests(const Position& position ,shared_ptr<Figure> figure)
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

namespace figuretests
{
	TEST_CLASS(PAWN_TESTS)
	{
		TEST_METHOD(GENERATE_PAWN_MOVES_START_POSITION)
		{
			_initializeRays();
			Position start_position;
			
			shared_ptr<Figure> white_pawn = shared_ptr<Pawn>(new Pawn(WHITE));
			shared_ptr<Figure> black_pawn = shared_ptr<Pawn>(new Pawn(BLACK));

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

			shared_ptr<Figure> white_knight = shared_ptr<Knight>(new Knight(WHITE));
			shared_ptr<Figure> black_knight = shared_ptr<Knight>(new Knight(BLACK));

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
	};

	TEST_CLASS(BISHOP_TESTS)
	{
		TEST_METHOD(GENERATE_BISHOP_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_bishop = shared_ptr<Bishop>(new Bishop(WHITE));
			shared_ptr<Figure> black_bishop = shared_ptr<Bishop>(new Bishop(BLACK));

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

			shared_ptr<Figure> white_rook = shared_ptr<Rook>(new Rook(WHITE));
			shared_ptr<Figure> black_rook = shared_ptr<Rook>(new Rook(BLACK));

			RawMoves white_moves = GenerateMoves_tests(start_position, white_rook);
			RawMoves black_moves = GenerateMoves_tests(start_position, black_rook);

			const U64 EXPECTED_SILENT_MOVES = 0;
			const U64 EXPECTED_CAPTURE_MOVES = 0;

			Assert::AreEqual(white_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_SILENT_MOVES);
			Assert::AreEqual(white_moves.silents, EXPECTED_CAPTURE_MOVES);
			Assert::AreEqual(black_moves.silents, EXPECTED_CAPTURE_MOVES);
		}
	};

	TEST_CLASS(QUEEN_TESTS)
	{
		TEST_METHOD(GENERATE_QUEEN_MOVES_START_POSITION)
		{
			_initializeRays();

			Position start_position;

			shared_ptr<Figure> white_queen = shared_ptr<Queen>(new Queen(WHITE));
			shared_ptr<Figure> black_queen = shared_ptr<Queen>(new Queen(BLACK));

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

			shared_ptr<Figure> white_king = shared_ptr<King>(new King(WHITE));
			shared_ptr<Figure> black_king = shared_ptr<King>(new King(BLACK));

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

namespace movelisttests
{
}


