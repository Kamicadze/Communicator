#pragma once
#include "IMenu.h"

class CMenu: public IMenu
{
    private:
        static int sockfd;

    public:
        CMenu();
        ~CMenu();
        void mainMenu();
        int registerForm();
        int loggingMenu();
        void deletingMenu(){};
        void invitationList(){};
        void createMenu(){};
        int startingMenu();
        void setSock(int);
        int menuLoop();
};


