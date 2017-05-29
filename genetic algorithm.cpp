// ConsoleApplication5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define POP 8

int getRand(int mod)
{
	if (mod == 0) return 0;
	else return rand() % mod;
}

void printArray(int array[], int n)
{
	char board[1000][1000];
	int x, y;
	for (x = 0; x<n; x++)
	{
		board[x][array[x]] = 'Q';
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
}

int getWeight(int array[], int n)
{
	int weight = 28;
	int queen;
	for (queen = 0; queen<n; queen++)								//for each queen
	{
		int nextqueen;
		for (nextqueen = queen + 1; nextqueen<n; nextqueen++)		//for each of the other queens (nextqueen = queen to avoid counting pairs twice)
		{
			if (array[queen] == array[nextqueen] || abs(queen - nextqueen) == abs(array[queen] - array[nextqueen]))		//if conflict
			{
				weight--;
			}
		}
	}
	return weight;
}

void geneticAlgorithm(int n) 
{
	int population[POP][100];
	int children[POP][100];
	int weightProb[224] = {};
	int wpl = 0;						//weightProb[] length
	double mutProb = 0.2;				//higher prob yields faster times. works decently anyways. bug: prob = 0
	int done = 0;
	int i;
	for (i = 0; i<POP; i++) 
		for (int j = 0; j<n; j++) 
			population[i][j] = getRand(n);
	while (done == 0) 
	{
		for (i = 0; i<POP; i++) 
		{
			if (getWeight(children[i],n) == 28) 
			{
				printArray(children[i],n);
				done = 1;
			}
		}

		for (i = 0; i<wpl; i++) 
			weightProb[i] = (int)NULL;			//clear weightprob
		wpl = 0;

		//weighted probability distribution
		for (i = 0; i<POP; i++) 
		{
			int w = getWeight(population[i], n);
			for (int j = 0; j<w; j++) 
			{
				weightProb[wpl] = i;			//fill array with member number w times
				wpl++;
			}
		}

		//reproduce
		for (i = 0; i<POP; i += 2) 
		{
			int par1 = weightProb[getRand(wpl)];
			int par2 = weightProb[getRand(wpl)];
			int split = getRand(n);
			//crossover
			for (int j = 0; j<split; j++) 
			{
				children[i][j] = population[par1][j];
				children[i + 1][j] = population[par2][j];
			}
			for (int j = split; j<n; j++) 
			{
				children[i][j] = population[par2][j];
				children[i + 1][j] = population[par1][j];
			}
			//mutation
			if (getRand(1000000) <= mutProb * 1000000) 
			{
				int child = getRand(2);
				if (child == 0) 
					children[i][getRand(n)] = getRand(n);
				else 
					children[i + 1][getRand(n)] = getRand(n);
			}
		}
		for (i = 0; i<POP; i++) 
			for (int j = 0; j<n; j++) 
				population[i][j] = children[i][j];
		wpl = 0;
	}
}

int main(int argc, const char * argv[]) 
{
	srand((unsigned int)time(NULL));  //seed random
	int total = 0;
	clock_t start, finish;
	int n;
	printf("Enter  number \'N\', \'N\' indicate numbers of queens in \"N * N\" chess board: \n");
	scanf("%d", &n);

	for (int i = 0; i < 10; i++)
	{
		start = clock();
		geneticAlgorithm(n);
		finish = clock();
		printf("\ntime taken = %d ms\n", finish - start);
		total = total + (finish - start);
	}
	printf("\nAverage time taken for generating 10 solutions is %d ms\n", total / 10);
	return 0;
}
