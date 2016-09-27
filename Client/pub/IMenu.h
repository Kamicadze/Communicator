#pragma once
class IMenu
{
    private:

    public:
        virtual ~IMenu(){}
        virtual void mainMenu()=0;
        virtual int registerForm()=0;
        virtual int loggingMenu()=0;
        virtual void deletingMenu()=0;
        virtual void invitationList()=0;
        virtual void createMenu()=0;
        virtual int startingMenu()=0;
        virtual int menuLoop()=0;
};
