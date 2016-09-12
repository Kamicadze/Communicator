#include "../inc/CWorkQueue.h"
#include <iostream>

int main()
{
	int x=10;
	CWorkQueue o_wq;
	//o_wq.m_addTask((ITask*)(x));
	std::cout<<o_wq.m_hasWork()<<std::endl;
	std::cout<<"main"<<std::endl;	
	return 0;
}

