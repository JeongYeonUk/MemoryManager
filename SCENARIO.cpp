#include "SCENARIO.h"
#include "MEMORY.h"

void Scenario::Scenario_ONE()
{
	system("cls");
	MemoryManager MM(100);
	Addr a = MM.First_Fit_MyAlloc(10);

	Addr b = MM.First_Fit_MyAlloc(20);

	Addr c = MM.First_Fit_MyAlloc(10);

	MM.OutputMemory();

	Addr d = MM.First_Fit_MyAlloc(70);

	MM.OutputMemory();

	MM.MyFree(d);
	system("pause");
}
void Scenario::Scenario_TWO()
{
	system("cls");
	MemoryManager MM(100);
	Addr a, b, c, d;

	a = MM.First_Fit_MyAlloc(10);

	b = MM.First_Fit_MyAlloc(20);

	c = MM.First_Fit_MyAlloc(10);

	d = MM.First_Fit_MyAlloc(15);

	MM.OutputMemory();

	MM.MyFree(a);

	MM.MyFree(c);

	MM.OutputMemory();

	Addr e = MM.First_Fit_MyAlloc(65);
	system("pause");
}
void Scenario::Scenario_THREE()
{
	system("cls");
	MemoryManager MM(100);
	Addr a = MM.First_Fit_MyAlloc(10);

	Addr b = MM.First_Fit_MyAlloc(5);

	Addr c = MM.First_Fit_MyAlloc(10);

	Addr d = MM.First_Fit_MyAlloc(5);

	Addr e = MM.First_Fit_MyAlloc(10);

	MM.MyFree(a);

	MM.MyFree(d);

	MM.OutputMemory();

	Addr f = MM.Best_Fit_MyAlloc(3);

	MM.OutputMemory();
	system("pause");
}