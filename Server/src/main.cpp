#include "CWorkQueue.h"
#include "CThPool.h"
#include "CDatabaseHandler.h"
#include <iostream>
#include <cstdio>

#include "Globals.h"
#include "CConnectionHandler.h"

//#include <gtest/gtest.h>
using namespace std;


int main()
{
	string login="kami";
	string password="123456";
	CThPool *tp= new CThPool(10);
	tp->addTask(new CConnectionHandler(2, tp));
	while(1)
	{
		//TODO: infinite loop which will break after getting kill sig	
	}

	tp->finish();
	delete tp;
	return 0;
}

