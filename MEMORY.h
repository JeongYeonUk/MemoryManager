#pragma once
#include "MEMORY MANAGEMENT.h"
#include "FUNCTION.h"
#include "DISPLAY.h"
class MemoryManager : public Function, public Display
{
private:
	int iMemory;
	PLIST Avail = new LIST;
	PLIST Used = new LIST;
	bool JudgeAlloc(int);
	bool JudgeFree(Addr);
	virtual void InitList(PLIST, int);
	virtual void AllocSuccess();
	virtual void AllocFail_Memory();
	virtual void FreeSuccess();
	virtual void FreeFail();
	virtual void AllocFail_DisContinuous();
	virtual void AllocFail_WrongInput();
	virtual void Combine();
	virtual void AvailInsert(PNODE);
	virtual void UsedInsert(PNODE);
public:
	MemoryManager(int Memory)
	{
		iMemory = Memory;
		InitList(Avail, iMemory);
		InitList(Used, 0);
	}
	~MemoryManager() {};	
	Addr First_Fit_MyAlloc(int);
	Addr Best_Fit_MyAlloc(int);
	void MyFree(Addr);
	void OutputMemory();	
};