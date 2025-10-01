// main.cpp
#if defined(_WIN32) || defined(_WIN64)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to close this window . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{	
	const int COLS = 5;
	const int ROWS = 5;

	
	SmartPlayer sp1("genius");
	BadPlayer bp("dumb");
	Game g1(4, 3, 2, &sp1, &bp);
	g1.play();

}