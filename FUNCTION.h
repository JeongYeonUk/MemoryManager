#pragma once
#include "MEMORY MANAGEMENT.h"
class Function
{
private:
	virtual void InitList(PLIST, int) = 0;
	virtual void Combine() = 0;
	virtual void AvailInsert(PNODE) = 0;
	virtual void UsedInsert(PNODE) = 0;
};