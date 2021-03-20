#include "Player.h"

#define WIN 99999

static int DEPTH;

Player::Player(Game* game, int color)
{
	game_ = game;
	color_ = color;
}

void Player::_addKillerMove(int ply, int move)
{
	auto searchIter = find(killerMoves_[ply].begin(), killerMoves_[ply].end(), move);
	if (searchIter == killerMoves_[ply].end())
	{
		for (int i = killerMoves_[ply].size() - 2; i >= 0; --i)
			killerMoves_[ply][i + 1] = killerMoves_[ply][i];
		killerMoves_[ply][0] = move;
	}
}

void Player::_sortMoveList(int ply, MoveList& moveList)
{
	//Find killer moves and set them on top of the list 
	for (int i = 0; i < killerMoves_[ply].size(); ++i)
	{
		int killer = killerMoves_[ply][i];
		for (auto iter = moveList.silent.begin(); iter != moveList.silent.end();)
		{
			if (*iter == killer)
			{
				moveList.killer.push_back(*iter);
				iter = moveList.silent.erase(iter);
			}
			else
				iter++;
		}
	}		
}

int Player::alphaBeta(int depth, int alpha, int beta)
{
	DEPTH = depth;
	killerMoves_ = vector<vector<int>>(depth, vector<int>(3));
	return _alphaBeta(depth, alpha, beta, color_);
}

int ply = 0;
int Player::_alphaBeta(int depth, int alpha, int beta, int color)
{
	if (depth == 0) return _quies(alpha, beta, color);

	MoveList moveList = game_->getMoves(color);
	_sortMoveList(ply, moveList);
	int move = moveList.next();

	int best_move = move;

	while (move && alpha < beta) {

		game_->makeMove(move);

		int temp = WIN;
		ply++;
		if (!game_->isGameOver())
			temp = -_alphaBeta(depth - 1, -beta, -alpha, color == WHITE ? BLACK : WHITE);

		game_->undoMove(move);
		ply--;

		if (temp > alpha) {
			alpha = temp;
			best_move = move;
			_addKillerMove(ply, best_move);
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

	string notation = game_->saveNotationOfMove(move);
	cout << notation<<endl;
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

