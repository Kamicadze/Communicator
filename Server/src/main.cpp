#include "CWorkQueue.h"
#include "CThPool.h"
#include "CDatabaseHandler.h"
#include <iostream>
#include <cstdio>
#include "CTaskTMP.h"
#include "Globals.h"
//#include <gtest/gtest.h>
using namespace std;


int main()
{
	string login="kami";
	string password="123456";
	CThPool *tp= new CThPool(1);
	tp->addTask(new CDatabaseHandler(1, login, password));
	tp->finish();
	cout<<"main"<<endl;	
	delete tp;
	return 0;
}

