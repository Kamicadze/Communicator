#pragma once
#include "ITask.h"
#include <iostream>
#include <cstdio>
#include "Globals.h"
using namespace std;

class CTaskTMP: public ITask
{
public:
	CTaskTMP(int);
	~CTaskTMP();
	void run();
	int showTask();
	void met2();
	void met1(char);
	int flag;
};
