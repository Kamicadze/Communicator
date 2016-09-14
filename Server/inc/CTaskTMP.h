#pragma once
#include "ITask.h"
#include <iostream>
#include <cstdio>
#include "Globals.h"
using namespace std;

class CTaskTMP: public ITask
{
public:
	CTaskTMP();
	~CTaskTMP();
	void run();
	int showTask();
};
