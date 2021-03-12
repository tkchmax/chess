#include "Player.h"

#define WIN 99999

static int DEPTH;

Player::Player(Game* game, int color)
{
	game_ = game;
	color_ = color;
}

int Player::alphaBeta(int depth, int alpha, int beta)
{
	DEPTH = depth;
	return _alphaBeta(depth, alpha, beta, color_);
}

int Player::_alphaBeta(int depth, int alpha, int beta, int color)
{
	if (depth == 0) return _quies(alpha, beta, color);

	MoveList moveList = game_->getMoves(color);

	int move = moveList.next();

	int best_move = move;
	if (game_->getFigureBoard(KING, BLACK) & (1ULL << 3))
		cout << "";

	while (move && alpha < beta) {
		game_->makeMove(move);

		int temp = WIN;
		if (!game_->isGameOver())
			temp = -_alphaBeta(depth - 1, -beta, -alpha, color == WHITE ? BLACK : WHITE);

		game_->undoMove(move);

		if (temp > alpha) {
			alpha = temp;
			best_move = move;
		}
		move = moveList.next();
	}

	if (depth == DEPTH) { //!!!
		applyMove(best_move);
		cout << "alpha " << alpha << endl << endl;
	}
	return alpha;
}

void Player::applyMove(int move)
{
	int move_color = READ_COLOR(move);
	int move_figure = READ_FIGURE(move);
	int move_from = READ_FIGURE(move);

	if (move_figure == KING)
	{
		game_->setIsKingMoved(move_color, true);
	}
	else if (move_figure == ROOK)
	{
		int rookHStartPos = (move_color == WHITE ? 7 : 63);
		int rookAStartPos = (move_color == WHITE ? 0 : 56);
		if (move_from == rookHStartPos)
			game_->setIsRshRookMoved(move_color, true);
		else if (READ_FROM(move) == rookAStartPos)
			game_->setIsLshRookMoved(move_color, true);
	}
	int opColor = (move_color == WHITE ? BLACK : WHITE);


	cout << "eval before make move\t" << game_->evaluate(move_color) << endl;
	ShowListItem(move);

	game_->makeMove(move);

	//madedMoves[move_color].push_back(move);

	cout << "eval after make move\t" << game_->evaluate(move_color) << endl;
}

int Player::_quies(int alpha, int beta, int color)
{

	int eval = game_->evaluate(color);
	if (eval > alpha)
		alpha = eval;

	MoveList forcedMoves = game_->getMoves(color, MOVE_TYPE_TAKE);

	if (forcedMoves.capture.empty())
		return alpha;

	int move = forcedMoves.next();
	while (move && alpha < beta)
	{
		game_->makeMove(move);
		eval = -_quies(-beta, -alpha, color == WHITE ? BLACK : WHITE);
		game_->undoMove(move);

		if (eval > alpha)
			alpha = eval;

		move = forcedMoves.next();
	}
	return alpha;
}
