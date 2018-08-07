#pragma once
#include "MEMORY MANAGEMENT.h"

class Display
{
private:
	virtual void AllocSuccess() = 0;
	virtual void AllocFail_Memory() = 0;
	virtual void FreeSuccess() = 0;
	virtual void FreeFail() = 0;
	virtual void AllocFail_DisContinuous() = 0;
	virtual void AllocFail_WrongInput() = 0;
};