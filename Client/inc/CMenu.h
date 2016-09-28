#pragma once
#include "IMenu.h"

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
        void deletingMenu(){};
        void invitationList(){};
        void createMenu(){};
        int startingMenu();
        void setSock(int);
        int menuLoop();
        int publicMenu(std::string);
};


