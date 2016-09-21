#include "CDatabaseHandler.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <string.h>
using namespace std;

CDatabaseHandler::CDatabaseHandler(int flag, string login)
	:m_userLogin(login),
	m_flag(flag)
{
	pthread_mutex_init(&dbmtx, 0);
	pthread_cond_init(&dbcond, 0);
}

CDatabaseHandler::CDatabaseHandler()
{
	pthread_mutex_init(&dbmtx, 0);
	pthread_cond_init(&dbcond, 0);
}

CDatabaseHandler::CDatabaseHandler(int flag, string login, string password)
	:m_flag(flag),
	m_userLogin(login),
	m_userPassword(password)
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
	if(true==authenticate(login, password))
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
	else
	{
		//TODO: error handling
	}
}
bool CDatabaseHandler::authenticate(string log, string password)
{
	ifstream dbFile("../db/database.txt");
	string line, login, pass, onl, ip;
	if(dbFile)
	{
		while((getline(dbFile, line)))
		{
		istringstream ss(line);

		ss >> login >> pass >> onl >> ip;

		if(login==log && pass==password)
		{
			return true;			
		}
		


		}
		return false;
	}

}
bool CDatabaseHandler::findUser(string ln)
{
	ifstream dbFile("../db/database.txt");
	string line, login, pass, onl, ip;
	if(dbFile)
	{
		while((getline(dbFile, line)))
		{
		istringstream ss(line);

		ss >> login >> pass >> onl >> ip;

		if(login==ln)
		{
			return true;			
		}
		


		}
		return false;
	}

}
bool CDatabaseHandler::isOnline(string log)
{
	ifstream dbFile("../db/database.txt");
	string line, login, pass, onl, ip;
	if(dbFile)
	{
		while((getline(dbFile, line)))
		{
		istringstream ss(line);

		ss >> login >> pass >> onl >> ip;

		if(login == log && onl=="yes")
		{
			return true;			
		}
		


		}
		return false;
	}

}
void CDatabaseHandler::run()
{
	switch(m_flag)
	{
		case 1:
			createUser(m_userLogin, m_userPassword);
			break;

		case 2:
			deleteUser(m_userLogin, m_userPassword);
			break;

		case 3:
			authenticate(m_userLogin, m_userPassword);
			break;

		case 4:
			findUser(m_userLogin);
			break;

		case 5:
			isOnline(m_userLogin);
			break;	
	}
}
