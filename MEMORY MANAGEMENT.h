#pragma once

#include <iostream>
#include <windows.h>
using namespace std;

enum MemoryMenu
{
	MM_NONE,
	MM_ONE,
	MM_TWO,
	MM_THREE,
	MM_EXIT
};

typedef struct _tagMemory
{
	int iStart;
	int iSize;
}MEMORY;
typedef struct _tagNode
{
	MEMORY tMemory;
	_tagNode *pPrev;
	_tagNode *pNext;
}NODE, *PNODE, *Addr;
typedef struct _tagList
{
	PNODE pHead;
	PNODE pTail;
	int iSeries;
}LIST, *PLIST;
static int InputInt()
{
	int iInput;
	cin >> iInput;

	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1024, '\n');
		return INT_MAX;
	}
	return iInput;
}
static int OutputMenu()
{
	system("cls");
	cout << "1. SCENARIO ONE(FREE FAIL)" << endl << endl;
	cout << "2. SCENARIO TWO(Memory DisContinuous)" << endl << endl;
	cout << "3. SCENARIO THREE(BEST-FIT)" << endl << endl;
	cout << "4. EXIT" << endl << endl;
	cout << "Select Menu : ";
	int iInput = InputInt();

	if (iInput <= MM_NONE || iInput > MM_EXIT)
		return MM_NONE;

	return iInput;
}
