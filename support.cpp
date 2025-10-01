#include "support.h"
#include "provided.h"

#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

bool checkVertical(const Scaffold& s, int N, int color) // check for N checkers in vertical line
{

	for (int i = 1; i <= s.cols(); i++) // for each column left to right
	{
		for (int j = 1; j <= s.levels(); j++) // bottom up
		{
			if (s.checkerAt(i, j) != color)
				continue;

			if (j + N - 1 > s.levels()) // if no more room for N in a row, stop scanning this column
				break;

			int count = 1;
			while (count < N && s.checkerAt(i, j + count) == color ) // check for consecutive
				count++;
			if (count == N)
				return true;

		}
	}

	return false;
}

bool checkHorizontal(const Scaffold& s, int N, int color) // similar logic as checkVertical
{

	for (int j = 1; j <= s.levels(); j++)
	{
		for (int i = 1; i <= s.cols(); i++)
		{
			
			if (s.checkerAt(i, j) != color)
				continue;

			if (i + N - 1 > s.cols())
				break;
			
			int count = 1;
			while (count < N && s.checkerAt(i + count, j) == color)
				count++;
			if (count == N)
				return true;
		}
	}

	return false;
}



bool checkDiagonal1(const Scaffold& s, int N, int color) // similar logic as before
{

	for (int j = 1; j <= s.levels(); j++)
	{
		for (int i = 1; i <= s.cols(); i++)
		{

			if (s.checkerAt(i, j) != color)
				continue;

			if (i + (N - 1) > s.cols() || j + (N - 1) > s.levels())
				break;

			int count = 1;
			while (count < N && s.checkerAt(i + count, j + count) == color)
				count++;
			if (count == N)
				return true;
		}
	}

	return false;
}



bool checkDiagonal2(const Scaffold& s, int N, int color) // similar logic as before
{

	for (int j = 1; j <= s.levels(); j++)
	{
		for (int i = s.cols(); i >= 1; i--)
		{

			if (s.checkerAt(i, j) != color)
				continue;

			if (i - (N - 1) < 1 || j + (N - 1) > s.levels())
				break;

			int count = 1;
			while (count < N && s.checkerAt(i - count, j + count) == color)
				count++;
			if (count == N)
				return true;
		}
	}

	return false;
}

bool checkAll(const Scaffold& s, int N, int color) // combined check
{
	return (checkVertical(s, N, color) || checkHorizontal(s, N, color) || checkDiagonal1(s, N, color) || checkDiagonal2(s, N, color));
}

int rating(const Scaffold& s, int N, int color) // ratings for terminal positions
{
	if (checkAll(s, N, color))
		return +1;
	if (checkAll(s, N, 1 - color))
		return -1;
	if (s.numberEmpty() == 0)
		return 0;
	return 0;
}