#include "Game.h"



Game::Game()
{
	firstKingMove_ = vector<int>(2);
	firstLshRookMove_ = vector<int>(2);
	firstRshRookMove_ = vector<int>(2);
	rawPGN_ = vector<string>();
	nHalfMoves = 0;
}

void Game::makeMove(int move)
{

	int move_figure = READ_FIGURE(move);
	int move_to = READ_TO(move);
	int move_from = READ_FROM(move);
	int move_type = READ_MOVE_TYPE(move);
	int move_capture = READ_CAPTURE(move);
	int move_color = READ_COLOR(move);

	int oppositeColor = (move_color == WHITE ? BLACK : WHITE);

	if (move_figure == KING && !isKingMoved_[move_color])
	{
		firstKingMove_[move_color] = move;
		isKingMoved_[move_color] = true;
	}
	else if (move_figure == ROOK)
	{
		int lshRookStartPosition = (move_color == WHITE) ? 0 : 56;
		int rshRookStartPosition = (move_color == WHITE) ? 7 : 63;
		if (!isLshRookMoved_[move_color] && move_from == lshRookStartPosition)
		{
			firstLshRookMove_[move_color] = move;
			isLshRookMoved_[move_color] = true;
		}
		else if (!isRshRookMoved_[move_color] && move_from == rshRookStartPosition)
		{
			firstRshRookMove_[move_color] = move;
			isRshRookMoved_[move_color] = true;
		}
	}


	//Set new figure position 
	figures_[move_color][move_figure]->moveFigure(move_from, move_to);
	figureFromCoord_[move_color][move_from] = 0;
	figureFromCoord_[move_color][move_to] = move_figure;

	//remove capture figure
	if (move_capture)
	{
		figures_[oppositeColor][move_capture]->removePiece(move_to);
		figureFromCoord_[oppositeColor][move_to] = NO_FIGURE;
	}

	switch (move_type)
	{
	case MOVE_TYPE_0_0:
		_makeShortCastling(move_color);
		break;
	case MOVE_TYPE_0_0_0:
		_makeLongCastling(move_color);
		break;
	case PAWN_TO_QUEEN:
		_makePawnTransform(QUEEN, move_to, move_color);
		break;
	case PAWN_TO_KNIGHT:
		_makePawnTransform(KNIGHT, move_to, move_color);
		break;
	case PAWN_TO_BISHOP:
		_makePawnTransform(BISHOP, move_to, move_color);
		break;
	case PAWN_TO_ROOK:
		_makePawnTransform(ROOK, move_to, move_color);
		break;
	}

	nHalfMoves++;
}

void Game::undoMove(int move)
{
	int move_figure = READ_FIGURE(move);
	int move_to = READ_TO(move);
	int move_from = READ_FROM(move);
	int move_type = READ_MOVE_TYPE(move);
	int move_capture = READ_CAPTURE(move);
	int move_color = READ_COLOR(move);

	int oppositeColor = (move_color == WHITE ? BLACK : WHITE);

	if (move_figure == KING && move == firstKingMove_[move_color])
	{
		isKingMoved_[move_color] = false;
		firstKingMove_[move_color] = 0;
	}
	else if (move_figure == ROOK)
	{
		if (move == firstLshRookMove_[move_color])
		{
			isLshRookMoved_[move_color] = false;
			firstLshRookMove_[move_color] = 0;
		}
		else if (move == firstRshRookMove_[move_color])
		{
			isRshRookMoved_[move_color] = false;
			firstRshRookMove_[move_color] = 0;
		}
	}


	figures_[move_color][move_figure]->moveFigure(move_to, move_from);
	figureFromCoord_[move_color][move_to] = 0;
	figureFromCoord_[move_color][move_from] = move_figure;

	//restore capture figure
	if (move_capture)
	{
		figures_[oppositeColor][move_capture]->setFigureOnSquare(move_to);
		figureFromCoord_[oppositeColor][move_to] = move_capture;
	}
	switch (move_type)
	{
	case MOVE_TYPE_0_0:
		_undoShortCastling(move_color);
		break;
	case MOVE_TYPE_0_0_0:
		_undoLongCastling(move_color);
		break;
	case PAWN_TO_QUEEN:
		_undoPawnTransform(move_from, QUEEN, move_to, move_color);
		break;
	case PAWN_TO_KNIGHT:
		_undoPawnTransform(move_from, KNIGHT, move_to, move_color);
		break;
	case PAWN_TO_BISHOP:
		_undoPawnTransform(move_from, BISHOP, move_to, move_color);
		break;
	case PAWN_TO_ROOK:
		_undoPawnTransform(move_from, ROOK, move_to, move_color);
		break;
	}

	nHalfMoves--;
}


int Game::evaluate(int color) const
{
	int materialEval = getMaterialEval(color);
	int strategyEval = getStrategyEval(color);
	int mobilityEval = getMobilityEval(color);

	int doubledPawnsEval = getDoubledPawnEval(color);
	//int doubledPawnsEval = 0;

	return materialEval + strategyEval + (0.5 * mobilityEval) + doubledPawnsEval;
}

int Game::getMaterialEval(int color) const
{
	int eval = 0;
	for (int type = PAWN; type <= KING; ++type)
	{
		eval += (figures_[WHITE][type]->getCount() * figures_[WHITE][type]->getCost())
			- (figures_[BLACK][type]->getCount() * figures_[BLACK][type]->getCost());

		/*	nWhiteFigures = figures_[WHITE][type]->getFigureCount();
			nBlackFigures = figures_[BLACK][type]->getFigureCount();

			switch (type)
			{
			case KING:
				eval += (nWhiteFigures * KING_VALUE) - (nBlackFigures * KING_VALUE);
				break;
			case QUEEN:
				eval += (nWhiteFigures * QUEEN_VALUE) - (nBlackFigures * QUEEN_VALUE);
				break;
			case ROOK:
				eval += (nWhiteFigures * ROOK_VALUE) - (nBlackFigures * ROOK_VALUE);
				break;
			case BISHOP:
				eval += (nWhiteFigures * BISHOP_VALUE) - (nBlackFigures * BISHOP_VALUE);
				break;
			case KNIGHT:
				eval += (nWhiteFigures * KNIGHT_VALUE) - (nBlackFigures * KNIGHT_VALUE);
				break;
			case PAWN:
				eval += (nWhiteFigures * PAWN_VALUE) - (nBlackFigures * PAWN_VALUE);
				break;
			}*/
	}

	if (color == WHITE) return eval;
	return -eval;
}

int Game::getStrategyEval(int color) const
{
	if (getSideFiguresCount(WHITE) + getSideFiguresCount(BLACK) < 14)
	{
		//if endgame began change strategy eval for kings
		figures_[WHITE][KING]->setPrioritySquares(&kingEndGamePriority);
		figures_[BLACK][KING]->setPrioritySquares(&kingEndGamePriority);
	}
	else
	{
		figures_[WHITE][KING]->setPrioritySquares(&whiteKingMiddleGamePriority);
		figures_[BLACK][KING]->setPrioritySquares(&blackKingMiddleGamePriority);
	}
	vector<int> whitePiecesSquares;
	vector<int> blackPiecesSquares;
	int eval = 0;
	for (int type = PAWN; type <= KING; ++type)
	{
		whitePiecesSquares = figures_[WHITE][type]->getPiecesSquares();
		blackPiecesSquares = figures_[BLACK][type]->getPiecesSquares();

		for (int i = 0; i < whitePiecesSquares.size(); ++i)
			eval += figures_[WHITE][type]->getPriorityEvalOnSquare(whitePiecesSquares[i]);
		for (int j = 0; j < blackPiecesSquares.size(); ++j)
			eval -= figures_[BLACK][type]->getPriorityEvalOnSquare(blackPiecesSquares[j]);
	}

	if (color == WHITE) return eval;
	return -eval;
}
int Game::getMobilityEval(int color) const
{
	int whiteMobility = 0;
	int blackMobility = 0;
	int oppositeColor = (color == WHITE) ? BLACK : WHITE;

	U64 moves = 0;
	U64 whitePieces = getSideBoard(WHITE);
	U64 blackPieces = getSideBoard(BLACK);

	for (int type = PAWN; type <= KING; ++type)
	{
		whiteMobility += figures_[WHITE][type]->getMobility(whitePieces, blackPieces);
		blackMobility += figures_[BLACK][type]->getMobility(blackPieces, whitePieces);
	}

	int eval = whiteMobility - blackMobility;
	if (color == WHITE)
		return eval;
	return -eval;
}

bool Game::isGameOver()
{
	if (figures_[WHITE][KING]->getBoard() == 0 || figures_[BLACK][KING]->getBoard() == 0)
		return true;
	return false;
}

void Game::setIsKingMoved(int color, bool isMoved)
{
	isKingMoved_[color] = isMoved;
}

void Game::setIsLshRookMoved(int color, bool isMoved)
{
	isLshRookMoved_[color] = isMoved;
}

void Game::setIsRshRookMoved(int color, bool isMoved)
{
	isRshRookMoved_[color] = isMoved;
}

bool isExpandedNotationNedded(const Game* game, int figure_type, int move)
{
	int color = READ_COLOR(move);
	int move_to = READ_TO(move);
	int move_from = READ_FROM(move);

	Position tempPosition(*game);
	auto figures = tempPosition.getFigures();
	figures[color][figure_type]->removePiece(move_from);
	MoveList mv = tempPosition.getMoves(color);

	int m;
	while (m = mv.next())
	{
		if (READ_FIGURE(m) == figure_type && READ_TO(m) == move_to)
			return true;
	}
	return false;
}

void Game::saveNotationOfMove(string move)
{
	rawPGN_.push_back(move);
}

void Game::saveNotationOfMove(int move)
{
	string notation = getNotationOfMove(move);
	saveNotationOfMove(notation);
}

string Game::getNotationOfMove(int move) const
{
	int figure = READ_FIGURE(move);
	int move_from = READ_FROM(move);
	int move_to = READ_TO(move);
	int move_type = READ_MOVE_TYPE(move);
	int capture = READ_CAPTURE(move);
	int move_color = READ_COLOR(move);
	int oppositeColor = (move_color == WHITE) ? BLACK : WHITE;
	string s_figure = "";
	string s_capture = "";
	string identify = "";
	string transform = "";

	stringstream notation;

	switch (move_type)
	{
	case PAWN_TO_KNIGHT:
		transform = "=N";
		break;
	case PAWN_TO_BISHOP:
		transform = "=B";
		break;
	case PAWN_TO_ROOK:
		transform = "=R";
		break;
	case PAWN_TO_QUEEN:
		transform = "=Q";
		break;
	case MOVE_TYPE_0_0:
		return "O-O";
		//rawPGN_.push_back(notation.str());
		return notation.str();
	case MOVE_TYPE_0_0_0:
		return "O-O-O";
		//rawPGN_.push_back(notation.str());
		return notation.str();
	}

	switch (figure)
	{
	case PAWN:
		if (move_type == MOVE_TYPE_TAKE)
		{
		}
		break;
	case KNIGHT:
	{
		s_figure = 'N';
		bool expandedNotation = isExpandedNotationNedded(this, KNIGHT, move);
		if (expandedNotation)
			identify = toCoord(move_from)[0];
		break;
	}
	case BISHOP:
	{
		s_figure = 'B';
		bool expandedNotation = isExpandedNotationNedded(this, BISHOP, move);
		if (expandedNotation)
			identify = toCoord(move_from)[0];
	}
	break;
	case ROOK:
	{
		s_figure = 'R';
		bool expandedNotation = isExpandedNotationNedded(this, ROOK, move);
		if (expandedNotation)
			identify = toCoord(move_from)[0];
		break;
	}
	case QUEEN:
	{
		s_figure = 'Q';
		bool expandedNotation = isExpandedNotationNedded(this, QUEEN, move);
		if (expandedNotation)
			identify = toCoord(move_from)[0];
	}

	break;
	case KING:
		s_figure = 'K';
		break;
	}

	if (figure == PAWN && move_type == MOVE_TYPE_TAKE) {
		s_capture = toCoord(move_from)[0];
		s_capture = s_capture + "x";
	}
	else if (move_type == MOVE_TYPE_TAKE) s_capture = "x";



	notation << s_figure << identify << s_capture << toCoord(move_to) << transform;
	//rawPGN_.push_back(notation.str());
	return notation.str();
}

string Game::getPGN() const
{
	stringstream pgn;
	int n = 0;
	for (int i = 0; i < rawPGN_.size(); i += 2)
	{
		n++;
		pgn << n << ". " << rawPGN_[i];
		if (i + 1 < rawPGN_.size())
			pgn << " " << rawPGN_[i + 1] << " ";
		if (n % 6 == 0) pgn << "\n";

	}
	return pgn.str();
}

string Game::removeLastNotation()
{
	string temp = rawPGN_.back();
	rawPGN_.pop_back();
	return temp;
}

int Game::removeLastMove()
{
	int temp = madedMove_.back();
	madedMove_.pop_back();
	return temp;
}

void Game::_makeShortCastling(int color)
{
	isRshRookMoved_[color] = true;

	//set new rook position
	int move_from = (color == WHITE ? 7 : 63);
	int move_to = (color == WHITE ? 5 : 61);
	if (figures_[WHITE][PAWN]->getBoard() == 0)
		cout << "";
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;

}

void Game::_makeLongCastling(int color)
{
	isLshRookMoved_[color] = true;

	//set new rook position
	int move_from = (color == WHITE ? 0 : 56);
	int move_to(color == WHITE ? 3 : 59);

	if (figures_[WHITE][PAWN]->getBoard() == 0)
		cout << "";
	figures_[color][ROOK]->moveFigure(move_from, move_to);
	figureFromCoord_[color][move_from] = 0;
	figureFromCoord_[color][move_to] = ROOK;

}

void Game::_makePawnTransform(int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->removePiece(transform_square);
	figures_[color][transform_in]->setFigureOnSquare(transform_square);

	figureFromCoord_[color][transform_square] = transform_in;
}

void Game::_undoShortCastling(int color)
{
	isRshRookMoved_[color] = false;

	//restore rook
	int move_from = (color == WHITE ? 7 : 63);
	int move_to = (color == WHITE ? 5 : 61);
	if (figures_[WHITE][PAWN]->getBoard() == 0)
		cout << "";
	figures_[color][ROOK]->moveFigure(move_to, move_from);
	figureFromCoord_[color][move_from] = ROOK;
	figureFromCoord_[color][move_to] = 0;

}

void Game::_undoLongCastling(int color)
{
	isLshRookMoved_[color] = false;
	//restore rook
	int move_from = (color == WHITE ? 0 : 56);
	int move_to = (color == WHITE ? 3 : 59);
	if (figures_[WHITE][PAWN]->getBoard() == 0)
		cout << "";
	bool b = figures_[color][ROOK]->moveFigure(move_to, move_from);
	figureFromCoord_[color][move_from] = ROOK;
	figureFromCoord_[color][move_to] = 0;
}

void Game::_undoPawnTransform(int move_from, int transform_in, int transform_square, int color)
{
	figures_[color][PAWN]->setFigureOnSquare(move_from);
	figures_[color][transform_in]->removePiece(transform_square);

	figureFromCoord_[color][transform_square] = 0;
}

int _countDoubledPawns(vector<int> squares, int color)
{
	int count = 0;
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		int forwardPawn = (color == WHITE) ? *iter + 8 : *iter - 8;
		vector<int>::iterator findIter = find(squares.begin(), squares.end(), forwardPawn);
		if (findIter != squares.end())
			count++;
	}
	return count;
}
int Game::getDoubledPawnEval(int color) const
{
	vector<int> pawnSquares = figures_[color][PAWN]->getPiecesSquares();
	int countDoubledPawns = _countDoubledPawns(pawnSquares, color);

	if (countDoubledPawns == 0)
	{
		int oppositeColor = (color == WHITE) ? BLACK : WHITE;
		vector<int> oppositePawnSquares = figures_[oppositeColor][PAWN]->getPiecesSquares();
		int oppositeCount = _countDoubledPawns(oppositePawnSquares, oppositeColor);
		if (oppositeCount)
			return oppositeCount * 10;
	}

	return -countDoubledPawns * 10;
}

string Game::toCoord(int number) const
{
	char rank = char((number / 8) + '1');
	char file = char(number % 8 + 'a');

	stringstream ss;
	ss << file << rank;

	string coord;
	ss >> coord;

	return coord;
}
