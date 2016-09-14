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
	for(int i=0;i<40;i++)
	{
		pthread_mutex_lock(&console_mutex);
		cout<<"work: "<<x*i<<"for thread: "<<pthread_self()<<endl;
		pthread_mutex_unlock(&console_mutex);
	}
//	sleep(10);
}

int CTaskTMP::showTask()
{
	cout<<"task"<<endl;
	return 0;
}

