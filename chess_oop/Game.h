#pragma once
#include "Position.h"
class Game : public Position
{
public:
	void makeMove();
	void UndoMove();



	int evaluate();

};

