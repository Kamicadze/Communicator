#include "../inc/CWorkQueue.h"
#include <iostream>
//#include <gtest/gtest.h>
using namespace std;

int main()
{
	int x=10;
	CWorkQueue o_wq;
	cout<<"has work: "<<o_wq.hasWork()<<endl;;
	o_wq.addTask((ITask*)(x));
	cout<<o_wq.hasWork()<<endl;
	o_wq.finished();
	o_wq.addTask((ITask*)(x));
	cout<<"main"<<endl;	
	//testing::InitGoogleTest(&argc, argv);
	return 0;
	//return RUN_ALL_TESTS();
}

