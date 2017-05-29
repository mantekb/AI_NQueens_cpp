// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INIT_TEMP 1000.0
#define FIN_TEMP 0.5
#define ALPHA 0.98
#define STEPS 1000

typedef struct {
	int solution[1000];
	float energy;
} memberType;

void tweakSolution(memberType *member, int n)
{
	int temp, x, y;

	x = rand() % n;

	do
	{
		y = rand() % n;
	} while (x == y);

	temp = member->solution[x];
	member->solution[x] = member->solution[y];
	member->solution[y] = temp;

}

void initSolution(memberType *member, int n)
{
	int i;

	for (i = 0; i<n; i++)
	{
		member->solution[i] = i;
	}

	for (i = 0; i<n; i++)
	{
		tweakSolution(member,n);
	}

}

void computeEnergy(memberType *member, int n)
{
	int i, j, x, y, tempx, tempy;
	
	char board[1000][1000];

	int conflicts;

	const int dx[20] = { -1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1 };

	const int dy[20] = { -1, 1, 1 , -1,-1, 1, 1 , -1,-1, 1, 1 , -1,-1, 1, 1 , -1,-1, 1, 1 , -1 };

	for (i = 0; i<n; i++)
	{
		board[i][member->solution[i]] = 'Q';
	}

	conflicts = 0;
	for (i = 0; i<n; i++)
	{
		x = i;
		y = member->solution[i];

		for (j = 0; j<20; j++)
		{
			tempx = x;
			tempy = y;

			while (1)
			{
				tempx += dx[j];
				tempy += dy[j];

				if (tempx<0 || tempx >= n || tempy<0 || tempy >= n)
				{
					break;
				}

				if (board[tempx][tempy] == 'Q')
					conflicts++;
			}
		}
	}

	member->energy = (float)conflicts;
}

void copySolution(memberType *dest, memberType *src, int n)
{
	int i;

	for (i = 0; i<n; i++)
	{
		dest->solution[i] = src->solution[i];
	}

	dest->energy = src->energy;
}

void emitSolution(memberType *member, int n)
{
	char board[1000][1000];

	int x, y;

	for (x = 0; x<n; x++)
	{
		board[x][member->solution[x]] = 'Q';
	}

	printf("\n board: \n");

	for (y = 0; y<n; y++)
	{
		for (x = 0; x<n; x++)
		{
			if (board[x][y] == 'Q')
				printf("Q ");
			else
				printf(". ");
		}
		printf("\n");

	}
	printf("\n\n");

}

int main()
{
	int n;
	printf("Enter  number \'N\', \'N\' indicate numbers of queens in \"N * N\" chess board: \n");
	scanf("%d", &n);

	srand(time(NULL));
	int total = 0;
	clock_t start, finish;

	for (int i = 0; i < 10; i++)
	{
		start = clock();
		int step, solution = 0, useNew, accepted;

		float temp = INIT_TEMP;
		memberType current, working, best;

		initSolution(&current, n);
		computeEnergy(&current, n);

		best.energy = 100.0;

		copySolution(&working, &current, n);

		while (temp > FIN_TEMP)
		{
			accepted = 0;

			for (step = 0; step < STEPS; step++);
			{
				useNew = 0;

				tweakSolution(&working, n);
				computeEnergy(&working, n);

				if (working.energy <= current.energy)
				{
					useNew = 1;
				}
				else
				{
					float test = rand() % 1;
					float delta = working.energy - current.energy;
					float calc = exp(-delta / temp);

					if (calc > test)
					{
						accepted++;
						useNew = 1;
					}
				}
			}

			if (useNew)
			{
				useNew = 0;
				copySolution(&current, &working, n);

				if (current.energy < best.energy)
				{
					copySolution(&best, &current, n);
					solution = 1;
				}

				else
				{
					copySolution(&working, &current, n);
				}

			}

			temp *= ALPHA;
		}
		if (solution)
		{
			emitSolution(&best, n);
		}

		finish = clock();
		printf("\ntime taken = %d ms\n", finish - start);
		total = total + (finish - start);
	}
	printf("\nAverage time taken for generating 10 solutions is %d ms\n", total / 10 );
	return 0;
}