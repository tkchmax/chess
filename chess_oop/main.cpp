#include "Position.h"
//#include "Figure.h"
#include <sstream>

void ShowBits(U64 bb)
{
	int bit;
	int count = 0;

	int i = 64 - 8;
	for (; i >= 0; i++) {
		bit = (bb >> i) & 1;
		cout << bit << '\t';
		count++;
		if (count == 8)
		{
			count = 0;
			i = i - 16;
			cout << endl;
		}
	}
	cout << endl << endl;
}


int main()
{
	Position p;
	ShowBits(p.getAtackRays(BLACK));
}

