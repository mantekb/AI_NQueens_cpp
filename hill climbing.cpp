#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>


using namespace std;

//print solution in console
void printBoardinTerminal(int *board, int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			if (j == board[i])
			{
				cout << 1 << " ";
			}
			else
			{
				cout << 0 << " ";
			}
		}
		cout << "\n";
	}
}

//The number of queens couples who are threatened themself
int evaluate(int *board, int len)
{
	int score = 0;
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (board[i] == board[j])
			{
				score++;
				continue;
			}
			if (board[i] - board[j] == i - j)
			{
				score++;
				continue;
			}
			if (board[i] - board[j] == j - i)
			{
				score++;
				continue;
			}
		}
	}
	return score;
}

//generate new state from current state 
int* generateBoard(int *board, int len)
{
	vector <int> choice;

	int temp;
	int score;
	int eval = evaluate(board, len);
	int k;

	int *boardOut;
	boardOut = new int[len];


	for (int i = 0; i < len; i++)
	{
		boardOut[i] = board[i];
	}

	for (int i = 0; i < len; i++)
	{
		choice.clear();

		choice.push_back(boardOut[i]);
		temp = boardOut[i];

		for (int j = 0; j < len; j++)
		{
			boardOut[i] = j;

			k = evaluate(boardOut, len);

			if (k == eval)
			{
				choice.push_back(j);
			}

			if (k < eval)
			{
				choice.clear();
				choice.push_back(j);
				eval = k;
			}
		}
		boardOut[i] = choice[rand() % choice.size()];
	}

	return boardOut;
}

//in this function , genarate new state by pervious function and if it has better value then replaces that by current state
bool findNextState(int *board, int len)
{
	int maineval = evaluate(board, len);

	int *tempBoard;

	tempBoard = generateBoard(board, len);

	if (evaluate(tempBoard, len) < maineval)
	{
		for (int p = 0; p < len; p++)
		{
			board[p] = tempBoard[p];
		}

		return  true;
	}

	return false;
}

// make random initial state , put one queen in each row
void initialRandomBoard(int * board, int len)
{
	bool access;
	int col;

	for (int i = 0; i < len; i++)
	{
		board[i] = rand() % len;
	}
}

//this function include a loop that call findNextState function , and do that until reach solution
//if findNextState function return NULL then we reset current state
void SolveNQueen(int len)
{
	int *board;
	board = new int[len];


	initialRandomBoard(board, len);

	while (evaluate(board, len) != 0)
	{
		if (!findNextState(board, len))
		{
			initialRandomBoard(board, len);
		}
	}

	cout << "\nAnwser for " << len << " queens: \n\n";
	printBoardinTerminal(board, len);

}


int main()
{
	int n, total = 0;
	clock_t start, finish;
	srand(time(NULL));

	cout << "Enter  number \'N\', \'N\' indicate numbers of queens in \"N * N\" chess board: \n";
	cin >> n;

	for (int i = 0; i < 10; i++)
	{
		start = clock();
		SolveNQueen(n);
		finish = clock();
		cout << "\ntime taken = " << finish - start<<"ms\n";
		total = total + (finish - start);
	}
	cout << "\nAverage time taken for generating 10 solutions is " << total / 10<<"ms\n";

	return 0;
}