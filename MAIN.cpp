#include "MEMORY.h"
#include "SCENARIO.h"

int main()
{
	/*while (true)
	{
		int iMenu = OutputMenu();

		if (iMenu == MM_EXIT)
			break;
		switch (iMenu)
		{
		case MM_ONE:
			Scenario::Scenario_ONE();
			break;
		case MM_TWO:
			Scenario::Scenario_TWO();
			break;
		case MM_THREE:
			Scenario::Scenario_THREE();
			break;		
		}
	}*/
	MemoryManager MM(100);

	Addr a = MM.First_Fit_MyAlloc(1);
	Addr b = MM.First_Fit_MyAlloc(2);
	Addr c = MM.First_Fit_MyAlloc(3);
	Addr d = MM.First_Fit_MyAlloc(4);
	Addr e = MM.First_Fit_MyAlloc(5);
	Addr f = MM.First_Fit_MyAlloc(6);
	Addr g = MM.First_Fit_MyAlloc(7);
	Addr h = MM.First_Fit_MyAlloc(8);
	Addr i = MM.First_Fit_MyAlloc(9);
	Addr j = MM.First_Fit_MyAlloc(10);
	MM.OutputMemory();

	MM.MyFree(b);
	MM.MyFree(d);
	MM.MyFree(f);
	MM.MyFree(h);	

	MM.OutputMemory();
	
	MM.MyFree(a);
	MM.OutputMemory();
	return 0;
}