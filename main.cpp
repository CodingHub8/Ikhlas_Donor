#include "Utilities.h"
#include "DBConnection.h"
#include "User.h"
#include "Admin.h"
#include "Item.h"
using namespace std;

// Item section start
void itemOptions(User& user) {
	system("cls");//clear text
	int choice;
	Item item;

	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------ ITEM ------------------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Add Item                            +" << endl;
	cout << "+ 2. Edit Item                           +" << endl;
	cout << "+ 3. Delete Item                         +" << endl;
	cout << "+ 0. Back                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	input(choice);

	switch(choice){
		case 1:
			item.addItem(user);
			break;
		case 2:
			break;
		case 3:
			system("cls");//clear text

			break;
		case 4:
			system("cls");//clear text

			break;
		case 0:
			return;//return to previous page
		default:
			cout << "Invalid choice. Please try again." << endl << endl;
	}
}


// Item section ends

// User section start
void userOptions(User&);
void donorOptions();
void recipientOptions();

void userMenu(){
	system("cls");//clear text

	int choice;
	User user;

	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------ USER ------------------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Login                               +" << endl;
	cout << "+ 2. Sign Up                             +" << endl;
	cout << "+ 0. Back                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";

	input(choice);

	switch(choice){
		case 1:
			cout << "+++++ USER LOGIN +++++" << endl << endl;
			if(user.login()){
				userOptions(user);
			} else {
				cout << "Invalid login credentials" << endl;
				system("pause");
			}
			break;
		case 2:
			cout << "+++++ USER SIGN UP +++++" << endl;
			user.signup();
			system("pause");
			break;
		case 0:
			return;//return to previous page
		default:
			cout << "Invalid choice. Please try again." << endl << endl;
	}

	userMenu();
}

void userOptions(User& user) {
	system("cls");//clear text
	int choice;

	cout << "WELCOME, " << user.getName() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	if (toLowerCase(user.getRole()) == "donor") {
		donorOptions();
	} else if (toLowerCase(user.getRole()) == "recipient") {
		recipientOptions();
	}
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	input(choice);

	switch(choice){
		case 1:
			if (user.getRole() == "donor") {
				itemOptions(user);
			} else if (user.getRole() == "recipient") {

			}
			break;
		case 2:
			if (user.getRole() == "donor") {

			} else if (user.getRole() == "recipient") {

			}
			break;
		case 3:
			system("cls");//clear text
			user.editProfile();
			break;
		case 4:
			system("cls");//clear text
			if (user.deleteProfile()) {
				userMenu();
			}
			break;
		case 0:
			return;//return to previous page
		default:
			cout << "Invalid choice. Please try again." << endl << endl;
	}

	userOptions(user);
}

void donorOptions() {
	cout << "------------------   DONOR   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Add Item                                 +" << endl;
	cout << "+ 2. Option 2                                 +" << endl;
}

void recipientOptions() {
	cout << "------------------ RECIPIENT ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Option 1                                 +" << endl;
	cout << "+ 2. Option 2                                 +" << endl;
}
// User section ends

// Admin section start
void adminOptions(Admin);

void adminMenu() {
	system("cls");//clear text

	int choice;
	Admin admin;

	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "----------------- ADMIN -----------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Login                              +" << endl;
	cout << "+ 2. Sign Up                            +" << endl;
	cout << "+ 0. Back                               +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";

	input(choice);

	switch(choice){
		case 1:
			cout << "+++++ ADMIN LOGIN +++++" << endl << endl;
			if(admin.login()){
				adminOptions(admin);
			} else {
				cout << "Invalid login credentials" << endl;
				system("pause");
			}
			break;
		case 2:
			cout << "+++++ ADMIN SIGN UP +++++" << endl;
			admin.signup();
			system("pause");
			break;
		case 0:
			return;//return to previous page
		default:
			cout << "Invalid choice. Please try again." << endl << endl;
	}

	adminMenu();
}

void adminOptions(Admin admin) {
	system("cls");//clear text
	int choice;

	cout << "WELCOME, " << admin.getUsername() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------   ADMIN   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Option 1                                 +" << endl;
	cout << "+ 2. Option 2                                 +" << endl;
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	input(choice);

	switch(choice){
		case 1:
			break;
		case 2:
			break;
		case 3:
			system("cls");//clear text
			admin.editProfile();
			break;
		case 4:
			system("cls");//clear text
			if (admin.deleteProfile()) {
				adminMenu();
			}
			break;
		case 0:
			return;//return to previous page
		default:
			cout << "Invalid choice. Please try again." << endl << endl;
	}

	adminOptions(admin);
}
// Admin section ends

void home() {
	system("cls");//clear text, data still remains
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "----------- IKHLAS DONOR SYSTEM ----------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. User                                +" << endl;
	cout << "+ 2. Admin                               +" << endl;
	cout << "+ 0. Exit                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
}

int main() {
	DBConnection::InitializeDatabase();
	system("pause");
	system("cls");

	int choice;

	do{
		home();
		input(choice);

		switch(choice){
			case 1://user
				userMenu();
				break;
			case 2://admin
				adminMenu();
				break;
			case 0:
				cout << "THANK YOU FOR USING IKHLAS DONOR! :)" << endl;
				mysql_close(conn);
				system("pause");
				exit(0);
			default:
				cout << "Invalid choice. Please try again." << endl << endl;
		}
	}while(true);
}