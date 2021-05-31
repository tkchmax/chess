#pragma once

#ifndef _GAME_
#define _GAME_

#include "Position.h"
class Game : public Position
{
public:
	Game();

	void makeMove(int move);
	void undoMove(int move);

	int evaluate(int color) const;

	int getMaterialEval(int color) const;
	int getStrategyEval(int color) const;
	int getMobilityEval(int color) const;
	int getDoubledPawnEval(int color) const;

	bool isGameOver();

	void setIsKingMoved(int color, bool isMoved);
	void setIsLshRookMoved(int color, bool isMoved);
	void setIsRshRookMoved(int color, bool isMoved);

	void saveNotationOfMove(string move);
	void saveNotationOfMove(int move);
	void saveMove(int move) { madedMove_.push_back(move); }
	string getNotationOfMove(int move) const;

	string getPGN() const;
	string removeLastNotation();
	int removeLastMove();

protected:
	void _makeShortCastling(int color);
	void _makeLongCastling(int color);
	void _makePawnTransform(int transform_in, int square, int color);
	
	void _undoShortCastling(int color);
	void _undoLongCastling(int color); 
	void _undoPawnTransform(int move_from, int transform_in, int square, int color);

	string toCoord(int number) const;

protected:
	bool game_over;

private:
	vector<int> firstKingMove_;
	vector<int> firstLshRookMove_;
	vector<int> firstRshRookMove_;

	vector<string> rawPGN_;
	vector<int> madedMove_;

	int nHalfMoves;

};

#endif // !_GAME_
;