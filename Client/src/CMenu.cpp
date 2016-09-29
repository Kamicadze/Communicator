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

int CMenu::publicMenu(std::string login)
{
    std::string buffer;
    SFrame frame;
    CConnectionHandler ch;
    std::string tmp;
    int whileFlag=1;


    std::cout<<"\033[2J\033[1;1H";
    std::cout<<"0. EXIT"<<std::endl;
    std::cout<<"Communication has been started"<<std::endl<<std::endl;
    std::cout<<"You can type in your message with maximum of 149 characters"<<std::endl;

    while(whileFlag)
    {
        std::getline(std::cin, buffer);
        if(149<buffer.length())
        {
            if(298<buffer.length())
            {
                std::cerr<<"Error: Message to long. Sending has been abort"<<std::endl;
            }
            else
            {

                tmp.clear();
                for(int i=0; i<149;i++)
                {
                    tmp[i]=buffer[i];
                }
                frame=ch.frameCreator(3, tmp, login, sockfd);
                if(0>(write(sockfd, &frame, sizeof(frame))))
                {
                    std::cerr<<"Error: cannot write to socket"<<std::endl;
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    getchar();
                    return 1;

                }
                tmp.clear();
                for(uint16_t i=149; i<298; i++)
                {
                    if(i>buffer.length())
                    {
                        tmp[i-149]=buffer[i];
                    }
                    else
                    {
                        break;
                    }

                }
                frame=ch.frameCreator(3, tmp, login, sockfd);
                if(0>(write(sockfd, &frame, sizeof(frame))))
                {
                    std::cerr<<"Error: cannot write to socket"<<std::endl;
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    getchar();
                    return 1;

                }
            }
        }
        else if(3>buffer.length() && (buffer.find("0") != std::string::npos))
        {
            whileFlag=0;
            frame=ch.frameCreator(5, buffer, login, sockfd);
            if(0>(write(sockfd, &frame, sizeof(frame))))
            {
                std::cerr<<"Error: cannot write to socket"<<std::endl;
                std::cout<<"Press ENTER to continue"<<std::endl;
                getchar();
                return 1;

            }

        }
        else
        {
            frame=ch.frameCreator(3, buffer, login, sockfd);
            if(0>(write(sockfd, &frame, sizeof(frame))))
            {
                std::cerr<<"Error: cannot write to socket"<<std::endl;
                std::cout<<"Press ENTER to continue"<<std::endl;
                getchar();
                return 1;

            }

        }
    }
    return 0;
}

int CMenu::createMenu(std::string login)
{
    SFrame frame;
    CConnectionHandler ch;
    std::string buff, invite;
    int tmp=0;

    std::cout<<"\033[2J\033[1;1H";

    frame=ch.frameCreator(8, buff, login, sockfd);
    if(0>(write(sockfd, &frame, sizeof(frame))))
    {
        std::cerr<<"Error: cannot write to socket"<<std::endl;
        std::cout<<"Press ENTER to continue"<<std::endl;
        getchar();
        return 1;

    }

    buff.clear();
    invite.clear();

    std::cout<<"Type in logins of users you want invite"<<std::endl;
    std::cout<<"Maximum 10 invites at once can be send"<<std::endl;
    std::cout<<"Press 0 if want to stop"<<std::endl;
    while(1)
    {
        tmp++;

        if(tmp==10)
        {
            std::cout<<"You have sent maximum number of invites"<<std::endl;
            break;
        }

        std::cin>>invite;

        if(3>invite.length() && (invite.find("0") != std::string::npos))
        {
            break;
        }

        buff.append(invite);
        buff.append("  ");
    }

    frame=ch.frameCreator(4, buff, login, sockfd);
    if(0>(write(sockfd, &frame, sizeof(frame))))
    {
        std::cerr<<"Error: cannot write to socket"<<std::endl;
        std::cout<<"Press ENTER to continue"<<std::endl;
        getchar();
        return 1;

    }


    chatMenu(login);



    return 0;
}


int CMenu::mainMenu(std::string login)
{
    uint16_t optionFlag=0;
    uint16_t retFlag=1;
    SFrame frame;
    CConnectionHandler ch;
    std::string buff;

    while(retFlag!=0)
    {
        std::cout<<"\033[2J\033[1;1H";
        std::cout<<"1. JOIN PUBLIC CHAT"<<std::endl;
        std::cout<<"2. CREATE PRIVATE CHAT ROOM"<<std::endl;
        std::cout<<"3. INVITE LIST"<<std::endl;
        std::cout<<"9. DELETE ACCOUNT"<<std::endl;
        std::cout<<"0. EXIT"<<std::endl;

        optionFlag=0;
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
                     optionFlag=0;               
                    //TODO: join public option, send a frame with only login and data type before accesing public menu
                    buff=login;
                    frame=ch.frameCreator(3, buff, login, sockfd);
                    if(0>(write(sockfd, &frame, sizeof(frame))))
                    {
                        std::cerr<<"Error: cannot write to socket"<<std::endl;
                        std::cout<<"Press ENTER to continue"<<std::endl;
                        getchar();
                        return 1;

                    }
                    publicMenu(login);

                    break;

                case 2:
                    optionFlag=0;
                    //TODO: create chat room
                    createMenu(login);
                    break;

                case 3:
                    optionFlag=0;
                    //TODO: invite list
                    invitationList(login);
                    break;

                case 9:
                    optionFlag=0;
                    //TODO:delete
                    deletingMenu(login);
                    //break;

                case 0:
                    //TODO: exit
                    buff="EXIT";
                    frame=ch.frameCreator(5, buff, login, sockfd);
                    if(0>(write(sockfd, &frame, sizeof(frame))))
                    {
                        std::cerr<<"Error: cannot write to socket"<<std::endl;
                        std::cout<<"Press ENTER to continue"<<std::endl;
                        getchar();
                        return 1;

                    }


                    return 0;
                    break;

                default:
                    break;
            }
        }
    }

    return 0;
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
                    flag=0;
                    flagChanged=false;

                    std::cout<<"Press ENTER to continue"<<std::endl;
                    std::cin.ignore();
                    getchar();
                    return 2;
                }
                else
                {
                    flag=0;
                    flagChanged=false;

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

void CMenu::deletingMenu(std::string login)
{
    std::string password;
    std::string buffer;
    SFrame frame;
    CConnectionHandler ch;


    std::cout<<"\033[2J\033[1;1H";
    std::cout<<"Type in your password to confirm you want to delete your account"<<std::endl;
    std::cout<<"OR typ 0 to EXIT"<<std::endl;

    std::cin>>password;

    if(3>buffer.length() && (buffer.find("0") != std::string::npos))
    {
    }
    else
    {
        buffer=login;
        buffer.append("  ");
        buffer.append(password);
        frame=ch.frameCreator(2, buffer, login, sockfd);
        if(0>(write(sockfd, &frame, sizeof(frame))))
        {
            std::cerr<<"Error: cannot write to socket"<<std::endl;
            std::cout<<"Press ENTER to continue"<<std::endl;
            getchar();
        }
        else
        {
            while(false==flagChanged)
            {
                ///waiting for answer from the server
            }
            if(flag==2)
            {
                flag=0;
                flagChanged=false;
                std::cout<<"Press ENTER to continue"<<std::endl;
                std::cin.ignore();
                getchar();

            } 
            else
            {
                flag=0;
                flagChanged=false;
                std::cout<<"Press ENTER to continue"<<std::endl;
                std::cin.ignore();
                getchar();
            }
        }

    }



}
int CMenu::chatMenu(std::string login)
{
    std::string buffer;
    SFrame frame;
    CConnectionHandler ch;
    std::string tmp;
    int whileFlag=1;


    std::cout<<"\033[2J\033[1;1H";
    std::cout<<"0. EXIT"<<std::endl;
    std::cout<<"Communication has been started"<<std::endl<<std::endl;
    std::cout<<"You can type in your message with maximum of 149 characters"<<std::endl;

    while(whileFlag)
    {
        std::getline(std::cin, buffer);
        if(149<buffer.length())
        {
            if(298<buffer.length())
            {
                std::cerr<<"Error: Message to long. Sending has been aborted"<<std::endl;
            }
            else
            {

                tmp.clear();
                for(int i=0; i<149;i++)
                {
                    tmp[i]=buffer[i];
                }
                frame=ch.frameCreator(4, tmp, login, sockfd);
                if(0>(write(sockfd, &frame, sizeof(frame))))
                {
                    std::cerr<<"Error: cannot write to socket"<<std::endl;
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    getchar();
                    return 1;

                }
                tmp.clear();
                for(uint16_t i=149; i<298; i++)
                {
                    if(i>buffer.length())
                    {
                        tmp[i-149]=buffer[i];
                    }
                    else
                    {
                        break;
                    }

                }
                frame=ch.frameCreator(4, tmp, login, sockfd);
                if(0>(write(sockfd, &frame, sizeof(frame))))
                {
                    std::cerr<<"Error: cannot write to socket"<<std::endl;
                    std::cout<<"Press ENTER to continue"<<std::endl;
                    getchar();
                    return 1;

                }
            }
        }
        else if(3>buffer.length() && (buffer.find("0") != std::string::npos))
        {
            whileFlag=0;
            frame=ch.frameCreator(5, buffer, login, sockfd);
            if(0>(write(sockfd, &frame, sizeof(frame))))
            {
                std::cerr<<"Error: cannot write to socket"<<std::endl;
                std::cout<<"Press ENTER to continue"<<std::endl;
                getchar();
                return 1;

            }

        }
        else
        {
            frame=ch.frameCreator(4, buffer, login, sockfd);
            if(0>(write(sockfd, &frame, sizeof(frame))))
            {
                std::cerr<<"Error: cannot write to socket"<<std::endl;
                std::cout<<"Press ENTER to continue"<<std::endl;
                getchar();
                return 1;

            }

        }
    }
    return 0;


}
int CMenu::invitationList(std::string login)
{
    std::string host;
    SFrame frame;
    CConnectionHandler ch;

    host.clear();
    std::cout<<"\033[2J\033[1;1H";
    std::cout<<"You have "<<pendingInvites.size()<<" pending invites"<<std::endl;
    if(pendingInvites.size()==0)
    {
        //TODO:handler to quit and to inform of no invites
        std::cin.ignore();
        getchar();
        return 0;
    }
    std::cout<<"Chose and type in login of the host"<<std::endl;
    std::cout<<"OR press 0 to exit"<<std::endl;
    for(auto it=pendingInvites.begin(); it!=pendingInvites.end(); it++)
    {
        std::cout<<*it<<std::endl;
    }
    std::cin>>host;
    if(3>host.length() && (host.find("0") != std::string::npos))
    {
        return 0;
    }

    frame=ch.frameCreator(6, host, login, sockfd);
    if(0>(write(sockfd, &frame, sizeof(frame))))
    {
        std::cerr<<"Error: cannot write to socket"<<std::endl;
        std::cout<<"Press ENTER to continue"<<std::endl;
        getchar();
        return 0;
    }

    pendingInvites.remove(host);

    chatMenu(login);

    return 0;
}

int CMenu::startingMenu()
{
    int opFlag=0;
    int sucFlag=1;
    SFrame frame;
    CConnectionHandler ch;
    std::string buff, login;


    while(sucFlag!=0)
    {
        std::cout<<"\033[2J\033[1;1H";

        std::cout<<"1. Login to your accout"<<std::endl;
        std::cout<<"2. Register new account"<<std::endl<<std::endl;
        std::cout<<"0. Exit"<<std::endl;
        std::cout<<"Choose an option"<<std::endl;

        opFlag=0;
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
                    opFlag=0;
                    sucFlag=loggingMenu();
                    
                    break;

                case 2:
                    opFlag=0;
                    sucFlag=registerForm();
                    break;

                case 0:
                    buff="EXIT";
                    login="0";
                    frame=ch.frameCreator(5, buff, login, sockfd);
                    if(0>(write(sockfd, &frame, sizeof(frame))))
                    {
                        std::cerr<<"Error: cannot write to socket"<<std::endl;
                        std::cout<<"Press ENTER to continue"<<std::endl;
                        getchar();
                        return 1;
                    }
                    return 0;
                    break;

                default:
                    break;



            }

        }
    }
    return 0;
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
        flag=0;
        flagChanged=false;
        std::cout<<"Press ENTER to continue"<<std::endl;
        std::cin.ignore();
        getchar();
        mainMenu(login);
        return 0;
    } 
    else
    {
        flag=0;
        flagChanged=false;
        std::cout<<"Press ENTER to continue"<<std::endl;
        std::cin.ignore();
        getchar();
        return 1;
    }

}
