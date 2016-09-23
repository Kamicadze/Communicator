#include "CDatabaseHandler.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <string.h>
using namespace std;

CDatabaseHandler::CDatabaseHandler()
{
	pthread_mutex_init(&dbmtx, 0);
	pthread_cond_init(&dbcond, 0);
}

CDatabaseHandler::~CDatabaseHandler()
{
	pthread_mutex_destroy(&dbmtx);
	pthread_cond_destroy(&dbcond);
}
void CDatabaseHandler::createUser(string login, string password)
{
	if(true==findUser(login))
	{
		cout<<"login already taken"<<endl;
		//TODO: error handling
	}
	else
	{
		//temporary ip, soon will be getting ip from frame
		string tmpip="100.100.100.100";
		pthread_mutex_lock(&dbmtx);
		ofstream fh;
		fh.open("../db/database.txt", ios::app);
		if(fh.is_open())
		{
			fh<<login<<"  "<<password<<"  no  "<<tmpip<<endl;
		
//			fh.write(static_cast<char>(login), 20);

			fh.close();
			pthread_mutex_unlock(&dbmtx);
		}
		else
		{
		//TODO: error handling
		}
	}

}
void CDatabaseHandler::deleteUser(string login, string password)
{
		string line, log, pass, onl, ip;
		pthread_mutex_lock(&dbmtx);
		ifstream dbFile("../db/database.txt");
		ofstream fh;
		fh.open("../db/tmp.txt", ios::app);
		if(fh.is_open())
		{
			while(dbFile >> log >> pass >> onl >> ip)
			{
				if(log==login)
				{
					//skip
				}
				else
				{
					fh << log <<"  "<<pass<<"  "<<onl<<"  "<<ip<<endl;
				}
			}
			fh.close();
			remove("../db/database.txt");
			rename("../db/tmp.txt", "../db/database.txt");
			pthread_mutex_unlock(&dbmtx);
		}
		
		
}
bool CDatabaseHandler::authenticate(string log, string password)
{
//	bool return_value=false;
	ifstream dbFile("database.txt");
	string line, login, pass, onl, ip;
	istringstream ss;
	cout<<dbFile.is_open()<<endl;
	if(dbFile.is_open())
	{
		cout<<"0"<<endl;
		while((getline(dbFile, line)))
		{
			cout<<"0.5"<<endl;
			ss.str(line);

			ss >> login >> pass >> onl >> ip;

			cout<<login<<"  "<<pass<<endl;

			if(login==log && pass==password)
			{
				cout<<"1";
			//	return_value= true;
				return true;
				break;
			}
		
		}
		return false;
	}
//	return false;
}
bool CDatabaseHandler::findUser(string ln)
{
	bool return_value=false;
	ifstream dbFile("../db/database.txt");
	string line, login, pass, onl, ip;
	istringstream ss;
	if(dbFile.is_open())
	{
		while(getline(dbFile, line))
		{
			ss.str(line);

			ss>>login>>pass>>onl>>ip;		

			if(login==ln)
			{
				return_value=true;
				break;
			}
		}	
	}
	return return_value;

}

