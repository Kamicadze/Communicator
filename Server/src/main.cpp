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
	CThPool *tp= new CThPool(2);
	CTaskTMP *tmp= new CTaskTMP;
	CTaskTMP *tmp2=new CTaskTMP;	
	tp->addTask(tmp);
	tp->addTask(tmp2);
	tp->finish();
	cout<<"main"<<endl;	
	delete tp;
	//testing::InitGoogleTest(&argc, argv);
	return 0;
	//return RUN_ALL_TESTS();
}

