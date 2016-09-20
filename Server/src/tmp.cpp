#include "CDatabaseHandler.h"
using namespace std;

int main()
{
	string login="kami";
	string pass="123456";
	string login2="tamii";
	CDatabaseHandler db;
	cout<<"user znaleziony: "<<db.findUser(login)<<endl;
	cout<<"prawidłowe pasy: "<<db.authenticate(login, pass)<<endl;
	cout<<"user online: "<<db.isOnline(login)<<endl;
	db.createUser(login, pass);
	db.createUser(login2, pass);
	db.deleteUser(login, pass);

	return 0;
}
