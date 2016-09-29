#pragma once
#include "IMenu.h"
#include <map>

class CMenu: public IMenu
{
    private:
        static int sockfd;

    public:
        CMenu();
        ~CMenu();
        int mainMenu(std::string);
        int registerForm();
        int loggingMenu();
        int chatMenu(std::string);
        int deletingMenu(std::string);
        int invitationList(std::string);
        int createMenu(std::string);
        int startingMenu();
        void setSock(int);
        int menuLoop();
        int publicMenu(std::string);

};


