#include "Utilities.h"
#include "DBConnection.h"
#include "User.h"
#include "Admin.h"
#include "Item.h"
#include "Recipient.h"
using namespace std;

// Item section start
void itemManagement(User& user) {
	system("cls");//clear text
	int choice;
	int viewChoice;
	string query;
	Item item;

	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------ ITEM ------------------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Add Item                            +" << endl;
	cout << "+ 2. Edit Item                           +" << endl;
	cout << "+ 3. View Item                           +" << endl;
	cout << "+ 4. Delete Item                         +" << endl;
	cout << "+ 0. Back                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1:// Add item
			if (item.addItem(user)) {
				cout << "Item added successfully!" << endl;
			} else {
				cout << "Failed to add item. Please try again." << endl;
			}
			break;
		case 2:// Edit item
			query = "SELECT * FROM item WHERE DONORID = '" + string(user.getID()) + "'";
			item.viewAllItems(query);//view all items to get ID
			item.viewItem();//view item to get name and price
			if (item.editItem(user)) {
				cout << "Item updated successfully." << endl;
			} else {
				cout << "Failed to update item. Please try again." << endl;
			}
			break;
		case 3:// View item
			do {
				system("cls");//clear text
				cout << "1. Search for an item" << endl;
				cout << "2. View all items" << endl;
				cout << "0. Back" << endl;
				cout << "Please choose from the option(s) above: ";
				inputint(viewChoice);

				if (viewChoice == 1) {
					item.viewItem();
				} else if (viewChoice == 2) {
					item.viewAllItems("SELECT * FROM item");
				} else if (viewChoice == 0) {
					break;
				} else {
					cout << "Invalid choice. Please try again." << endl << endl;
				}
				system("pause");//pause to view the result
			} while (true);
			itemManagement(user);
			break;
		case 4:// Delete item
			query = "SELECT * FROM item WHERE DONORID = '" + string(user.getID()) + "'";
			item.viewAllItems(query);//view all items to get ID
			if (item.deleteItem(user)) {
				cout << "Item deleted successfully." << endl;
			} else {
				cout << "Item deletion failed or cancelled." << endl;
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	system("pause");//pause to view the result
	itemManagement(user);
}


// Item section ends

// User section start
void userOptions(User&);
void donorOptions(User&);
void recipientOptions(User&);

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

	inputint(choice);

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
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	userMenu();
}

void userOptions(User& user) {
	system("cls");//clear text

	cout << "WELCOME, " << user.getName() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	if (toLowerCase(user.getRole()) == "donor") {
		donorOptions(user);
	} else if (toLowerCase(user.getRole()) == "recipient") {
		recipientOptions(user);
	}
}

// Donor
void donationReport(User&);

void donorOptions(User& user) {
	int choice;
	cout << "------------------   DONOR   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Item Management                          +" << endl;
	cout << "+ 2. View Donation Report                     +" << endl;
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1:
			itemManagement(user);
			break;
		case 2:
			donationReport(user);
			break;
		case 3:
			user.editProfile();
			break;
		case 4:
			if (user.deleteProfile()) {
				userMenu();
			}
			break;
		case 0: return;//return to menu page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}
	userOptions(user);
}

void donationReport(User& user){
	// TODO: Add logic
}

// Recipient
void recipientCreateRequest(User&);
void recipientRequestStatus(User&);

void recipientOptions(User& user) {
	int choice;
	Item item;

	cout << "------------------ RECIPIENT ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Request Donation                         +" << endl;
	cout << "+ 2. Request Status                           +" << endl;
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1://Request donation
			item.viewAllItems("SELECT * FROM item");//view all items
			recipientCreateRequest(user);//create request
			break;
		case 2://Request status
			recipientRequestStatus(user);
			break;
		case 3://Edit profile
			user.editProfile();
			break;
		case 4://Delete profile
			if (user.deleteProfile()) {
				userMenu();
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}
	userOptions(user);
}

void recipientCreateRequest(User& user) {
	cout << "Please enter the item ID to request: ";
}

void recipientRequestStatus(User& user) {
	// TODO: Add logic
}
// User section ends

// Admin section start
void adminOptions(Admin&);
void viewRequests(Admin&, const string&);
void processRecipientRequest();
void viewMonthlyReport(Admin&);

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

	inputint(choice);

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
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	adminMenu();
}

void adminOptions(Admin& admin) {
	system("cls");//clear text
	int choice;

	cout << "WELCOME, " << admin.getUsername() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------   ADMIN   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Approve Pending Request                  +" << endl;
	cout << "+ 2. View Monthly Report                      +" << endl;
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1://Approve pending requests
			viewRequests(admin, string("pending"));
			processRecipientRequest();
			break;
		case 2://View monthly report
			viewMonthlyReport(admin);
			break;
		case 3:
			admin.editProfile();
			break;
		case 4:
			if (admin.deleteProfile()) {
				system("pause");//pause to view the result
				return;
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	system("pause");
	adminOptions(admin);
}

void viewRequests(Admin& admin, const string &status) {
	vector<Request> requests;
	string query = "SELECT * FROM request WHERE STATUS = '" + status + "'";
	if (mysql_query(conn, query.c_str()) != 0) {
		cout << "Error fetching requests: " << mysql_error(conn) << endl;
		return;
	}
	res = mysql_store_result(conn);
	if (res) {
		cout << left << "| "
			 << setfill(' ') << setw(10) << "ID" << + "| "//ID
			 << setfill(' ') << setw(13) << "Recipient ID" << + "| "//recipientID
			 << setfill(' ') << setw(10) << "Item ID" << + "| "//itemID
			 << right
			 << setfill(' ') << setw(10) << "Amount" << + "| "//amount
			 << left
			 << setfill(' ') << setw(50) << "Request Address" << + "| "//requestAddress
			 << setfill(' ') << setw(20) << "Request Date" << + "| "//requestDate
			 << setfill(' ') << setw(50) << "Description" << + "| "//description set to 50 characters length max
			 << setfill(' ') << setw(10) << "Status" << + "|"//status
			 << endl;
		cout << setw(198) << setfill('-') << "" << endl;
		while ((row = mysql_fetch_row(res))) {
			string desc = row[6] ? row[6] : "NULL";
			if (desc.length() > 50) {
				desc.replace(47, 3, "...");
				desc = desc.substr(0, 50);
			}

			cout << left << "| "
				 << setfill(' ') << setw(10) << row[0] << "| "//ID
				 << setfill(' ') << setw(13) << row[1] << "| "//recipientID
				 << setfill(' ') << setw(10) << row[2] << "| "//itemID
				 << right
				 << setfill(' ') << setw(10) << row[3] << "| "//amount
				 << left
				 << setfill(' ') << setw(50) << row[4] << "| "//requestAddress
				 << setfill(' ') << setw(20) << row[5] << "| "//requestDate
				 << setfill(' ') << setw(50) << desc << "| "//description set to 50 characters length max
				 << setfill(' ') << setw(10) << row[7] << " |"//status
				 << endl;
		}
		cout << setw(198) << setfill('-') << "" << endl;
		cout << endl;
		mysql_free_result(res);
	}
}

void processRecipientRequest() {
	char requestID[10];
	string status;

	cout << "Please enter the request ID to process: ";
	cin >> requestID;
	strcpy(requestID, toUpperCase(requestID).c_str());

	cout << "Do you want to [1]Approve or [2]Reject this request?" << endl;
	int choice;
	inputint(choice);

	if (choice == 1) {
		status = "approved";
	} else if (choice == 2) {
		status = "failed";
	}

	string query =
			"SELECT r.AMOUNT as reqAmount, i.AMOUNT as itemAmount, r.ITEMID FROM request r JOIN item i ON r.ITEMID = i.ID WHERE r.ID = '"
			+ string(requestID) + "'";
	if (mysql_query(conn, query.c_str()) != 0) {
		cout << "Error fetching amounts: " << mysql_error(conn) << endl;
		return;
	}
	res = mysql_store_result(conn);
	if (res) {
		row = mysql_fetch_row(res);
		int reqAmount = atoi(row[0]);
		int itemAmount = atoi(row[1]);
		string itemID = row[2];
		int finalAmount = itemAmount - reqAmount;
		mysql_free_result(res);

		// Update request status
		query = "UPDATE request SET STATUS = '" + status + "' WHERE ID = '" + string(requestID) + "'";
		if (mysql_query(conn, query.c_str()) != 0) {
			cout << "Error updating request: " << mysql_error(conn) << endl;
			return;
		}

		// Update item amount
		query = "UPDATE item SET AMOUNT = " + to_string(finalAmount) + " WHERE ID = '" + itemID + "'";
		if (mysql_query(conn, query.c_str()) != 0) {
			cout << "Error updating item amount: " << mysql_error(conn) << endl;
			return;
		}
	}

	cout << "Request processed successfully!" << endl;
}

void viewMonthlyReport(Admin& admin) {
	// TODO: Add logic
}
// Admin section ends

int main() {
	DBConnection::InitializeDatabase();
	system("pause");

	int choice;

	do{
		system("cls");//clear text, data still remains
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "----------- IKHLAS DONOR SYSTEM ----------" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+ 1. User                                +" << endl;
		cout << "+ 2. Admin                               +" << endl;
		cout << "+ 0. Exit                                +" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
		cout << "Please choose from the option(s) above: ";
		inputint(choice);

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
			default: cout << "Invalid choice. Please try again." << endl << endl;
		}
	}while(true);
}