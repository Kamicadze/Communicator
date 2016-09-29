#pragma once
#include <string>
class IMenu
{
    private:

    public:
        virtual ~IMenu(){}
        virtual int mainMenu(std::string)=0;
        virtual int registerForm()=0;
        virtual int loggingMenu()=0;
        virtual void deletingMenu()=0;
        virtual int invitationList(std::string)=0;
        virtual int createMenu(std::string)=0;
        virtual int startingMenu()=0;
        virtual int menuLoop()=0;
        virtual int publicMenu(std::string)=0;
};
