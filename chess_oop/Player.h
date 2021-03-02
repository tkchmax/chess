#pragma once
#ifndef  _PLAYER_
#define _PLAYER_

#include "Game.h"


class Player
{
public:
	int alphaBeta();
private:
	int _alphaBeta();
	Game* game;
};


#endif
