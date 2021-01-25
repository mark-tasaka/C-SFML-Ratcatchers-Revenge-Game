
#include "stdafx.h"
#include "RatLair.h"
#include "Rat.h"


Rat* createHorde(int numRats, IntRect arena)
{
	Rat* rat = new Rat[numRats];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numRats; i++)
	{
		//which side should the Rat spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			//left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			//right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			//top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			//bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;

		default:
			x = 0;
			y = 0;
			break;
		}

		//bloater, crawler or runner
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);

		//spawn the new Rat into the array
		rat[i].spawn(x, y, type, i);

	}//end for loop

	return rat;

}