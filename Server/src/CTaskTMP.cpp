#include "CTaskTMP.h"
using namespace std;

CTaskTMP::CTaskTMP()
{
	pthread_mutex_lock(&console_mutex);
	cout<<"creating task"<<endl;
	pthread_mutex_unlock(&console_mutex);
}

CTaskTMP::~CTaskTMP()
//	:ITask()
{
	pthread_mutex_lock(&console_mutex);
	cout<<"task has ended"<<endl;
	pthread_mutex_unlock(&console_mutex);
}

void CTaskTMP::run()
{
	int x=30;
	long int y=0;
	for(int i=0;i<40;i++)
	{
		pthread_mutex_lock(&console_mutex);
		y=y+x*i;	
		pthread_mutex_unlock(&console_mutex);
	}
//	sleep(2);
}

int CTaskTMP::showTask()
{
	cout<<"task"<<endl;
	return 0;
}

