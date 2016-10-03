#include "CDatabaseHandler.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <sys/stat.h>

CDatabaseHandler::CDatabaseHandler()
{
    pthread_mutex_init(&m_dbmtx, 0);
    pthread_cond_init(&m_dbcond, 0);
}

CDatabaseHandler::~CDatabaseHandler()
{
    pthread_mutex_destroy(&m_dbmtx);
    pthread_cond_destroy(&m_dbcond);
}
bool CDatabaseHandler::createUser(std::string login, std::string password)
{
    bool returnValue=false;

    if(true==findUser(login))
    {
        std::cout<<"login already taken"<<std::endl;
        //TODO: error handling
    }
    else
    {
        //temporary ip, soon will be getting ip from frame
        std::string tmpip="100.100.100.100";
        pthread_mutex_lock(&m_dbmtx);
        std::ofstream fh;
        fh.open("../db/database.txt", std::ios::app);
        if(fh.fail())
        {
            std::cerr<<"Error: "<<strerror(errno)<<std::endl;     }
        else
        {
        //TODO: error handling

            fh<<login<<"  "<<password<<"  no  "<<tmpip<<std::endl;
        
            fh.close();
            pthread_mutex_unlock(&m_dbmtx);
            returnValue=true;
        }
    }
    return returnValue;
}
bool CDatabaseHandler::deleteUser(std::string login, std::string password)
{
        bool returnValue=false;

        std::string line, log, pass, onl, ip;
        pthread_mutex_lock(&m_dbmtx);
        std::ifstream dbFile("../db/database.txt");
        std::ofstream fh;
        fh.open("../db/tmp.txt", std::ios::app);
        if(fh.is_open() && dbFile.is_open())
        {
            while(dbFile >> log >> pass >> onl >> ip)
            {
                if(log==login)
                {
                    //skip
                }
                else
                {
                    fh << log <<"  "<<pass<<"  "<<onl<<"  "<<ip<<std::endl;
                }
            }
            fh.close();
            dbFile.close();
            remove("../db/database.txt");
            rename("../db/tmp.txt", "../db/database.txt");
            pthread_mutex_unlock(&m_dbmtx);
            returnValue=true;
        }
        else
        {
            std::cerr<<"Error: "<<strerror(errno)<<std::endl; 
        
        }

        return returnValue;
        
        
}
bool CDatabaseHandler::authenticate(std::string log, std::string password)
{
    std::cout<<"wchodze"<<std::endl;
    bool returnValue=false;
    std::ifstream dbFile("../db/database.txt");
    std::string line, login, pass, onl, ip;
    std::istringstream ss;
    dbFile.clear();
    dbFile.seekg(0, std::ios::beg);
    if(!dbFile.fail())
    {
        while(dbFile >> login >> pass >> onl >> ip)
        {
            if(true==(0==login.compare(log) && 0==pass.compare(password)))
            {
                returnValue= true;
                dbFile.close();
                break;
            }
            else
            {

                std::cout<<"szukany: "<<log<<" znaleziony: "<<login<<std::endl;
                login.clear();
                std::cout<<"szukanyp: "<<password<<" znalezionyp: "<<pass<<std::endl;

                pass.clear();


            }

        }

    }
    else
    {
        std::cerr<<"Error: "<<strerror(errno)<<std::endl; 
    }
    dbFile.close();
    return returnValue;
}
bool CDatabaseHandler::findUser(std::string ln)
{
    bool return_value=false;
    std::ifstream dbFile("../db/database.txt");
    std::string line, login, pass, onl, ip;
    if(!dbFile.fail())
    {
        while(dbFile>>login>>pass>>onl>>ip)
        {       

            if(login==ln)
            {
                return_value=true;
                break;
            }
        }   
    }
    else 
    {
        std::cerr<<"error file was not opened"<<std::endl;
    }

    dbFile.close();
    return return_value;

}

