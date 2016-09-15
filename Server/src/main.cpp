#include "CWorkQueue.h"
#include "CThPool.h"
#include <iostream>
#include <cstdio>
#include "CTaskTMP.h"
#include "Globals.h"
//#include <gtest/gtest.h>
using namespace std;


int main()
{
	CThPool *tp= new CThPool(10);
	for(int i=0;i<30;i++)
	{
		CTaskTMP *tmp= new CTaskTMP;
		tp->addTask(tmp);
	}
	tp->finish();
	cout<<"main"<<endl;	
	delete tp;
	//testing::InitGoogleTest(&argc, argv);
	return 0;
	//return RUN_ALL_TESTS();
}

