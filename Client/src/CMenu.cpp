#include "CMenu.h"
#include <iostream>
#include <cstdint>
#include <string>
#include "Globals.h"
#include "SFrame.h"
#include <unistd.h>
#include "CConnectionHandler.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{}

int CMenu::sockfd=0;

int CMenu::menuLoop()
{
    int retFlag;
    retFlag=startingMenu();
    if(0==retFlag)
    {
        return 0;
    }
    

    return 0;
}

void CMenu::mainMenu()
{
    uint16_t optionFlag=0;
    uint16_t retFlag=1;


    while(retFlag!=0)
    {
        std::cout<<"\033[2J\033[1;1H";
        std::cout<<"1. JOIN PUBLIC CHAT"<<std::endl;
        std::cout<<"2. CREATE PRIVATE CHAT ROOM"<<std::endl;
        std::cout<<"3. INVITE LIST"<<std::endl;
        std::cout<<"9. DELETE ACCOUNT"<<std::endl;
        std::cout<<"0. EXIT"<<std::endl;

        std::cin>>optionFlag;
        if(10<=optionFlag ||( 4<=optionFlag && 8>=optionFlag))
        {
            std::cout<<"Error: You chose wrong option"<<std::endl;
        }
        else
        {
            switch(optionFlag)
            {
                case 1:
                    //TODO: join public option
                    break;

                case 2:
                    //TODO: create chat room
                    createMenu();
                    break;

                case 3:
                    //TODO: invite list
                    invitationList();
                    break;

                case 9:
                    //TODO:delete
                    deletingMenu();
                    break;

                case 0:
                    //TODO: exit
                   // return 0;
                    break;

                default:
                    break;
            }
        }
    }


}

int CMenu::registerForm()
{
    std::string login, password, rpassword;
    std::cout<<"\033[2J\033[1;1H";
    std::cout<<"Welcom in register form"<<std::endl;
    std::cout<<"Please type in your login"<<std::endl;
    std::cout<<"Please note that your login can have maximum of 12 characters"<<std::endl;
    std::cin>>login;

    if(12<login.length())
    {
        std::cout<<"Your login is too long"<<std::endl;
        sleep(2);
        return 1;
    }
    else
    {
        std::cout<<"Please type in your password"<<std::endl;
        std::cout<<"Please note that your password can have maximum of 12 characters"<<std::endl;
        std::cin>>password;

        if(12<password.length())
        {
            std::cout<<"Your password is too long"<<std::endl;
            sleep(2);
            return 1;

        }
        else
        {
            std::cout<<"Please type in your password again"<<std::endl;
            std::cin>>rpassword;

            if(0!=password.compare(rpassword))
            {
                std::cout<<"Wrong password"<<std::endl;
                sleep(2);
                return 1;
            }
            else
            {
                std::string msg;
                msg=login;
                msg.append("  ");
                msg.append(password);
                SFrame frame;
                CConnectionHandler ch;
                frame=ch.frameCreator(7, msg, login, sockfd);
                if(0>(write(sockfd, &frame, sizeof(frame))))
                {
                    std::cerr<<"Error: cannot write to socket"<<std::endl;
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    getchar();
                    return 1;

                }
                while(false==flagChanged)
                {
                    ///waiting for answer from the server
                }
                if(flag==7)
                {
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    std::cin.ignore();
                    getchar();
                    return 2;
                }
                else
                {
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    std::cin.ignore();
                    getchar();
                    return 1;
                }

            }
        }

    }


    return 1;
}

void CMenu::setSock(int socketfd)
{
    CMenu::sockfd=socketfd;
}

int CMenu::startingMenu()
{
    int opFlag=0;
    int sucFlag=1;

    while(sucFlag!=0)
    {
        std::cout<<"\033[2J\033[1;1H";

        std::cout<<"1. Login to your accout"<<std::endl;
        std::cout<<"2. Register new account"<<std::endl<<std::endl;
        std::cout<<"0. Exit"<<std::endl;
        std::cout<<"Choose an option"<<std::endl;

        std::cin>>opFlag;

        getchar();

        if(10<=opFlag ||( 3>opFlag && 9<=opFlag))
        {
            std::cout<<"Error: You chose wrong option"<<std::endl;
        }
        else
        {
            switch(opFlag)
            {
                case 1:
                    sucFlag=loggingMenu();
                    break;

                case 2:
                    sucFlag=registerForm();
                    break;

                case 0:
                    return 0;
                    break;

                default:
                    break;



            }

        }
    }
    return 1;
}

int CMenu::loggingMenu()
{
    std::cout<<"\033[2J\033[1;1H";

    std::string login, password, msg;

    std::cout<<"Please type in your login"<<std::endl;
    std::cin>>login;
    std::cout<<"Please type in your password"<<std::endl<<std::endl;
    std::cin>>password;
    std::cout<<"Please wait. Authentication in progress"<<std::endl;

    msg=login;
    msg.append("  ");
    msg.append(password);
    SFrame frame;
    CConnectionHandler ch;
    frame=ch.frameCreator(1, msg, login, sockfd);
    if(0>(write(sockfd, &frame, sizeof(frame))))
    {
        std::cerr<<"Error: cannot write to socket"<<std::endl;
    }

    while(false==flagChanged)
    {
        ///waiting for answer from the server
    }
    if(flag==1)
    {
        std::cout<<"Press ENTER to continue"<<std::endl;
        std::cin.ignore();
        getchar();
        mainMenu();
        return 0;
    } 
    else
    {
        std::cout<<"Press ENTER to continue"<<std::endl;
        std::cin.ignore();
        getchar();
        return 1;
    }

}