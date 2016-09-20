#include "CTaskTMP.h"
using namespace std;

CTaskTMP::CTaskTMP(int f)
{
	this->flag=1;
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
	if(flag==1)
	{
		met2();
	}
	else if(flag==2)
	{
		met1('x');
	}
	else
	{
		cout<<"WTF"<<endl;
	}
}
void CTaskTMP::met1(char x)
{
	cout<<"methoda1"<<endl;
	cout<<x<<endl;
}
void CTaskTMP::met2()
{
	cout<<"methoda2"<<endl;
	int x=30;
	long int y=0;
	for(int i=0;i<40;i++)
	{
		pthread_mutex_lock(&console_mutex);
		y=y+x*i;	
		pthread_mutex_unlock(&console_mutex);
	}

}
int CTaskTMP::showTask()
{
	cout<<"task"<<endl;
	return 0;
}

