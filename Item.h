#ifndef ITEM_H
#define ITEM_H
#include <iostream>
using namespace std;

class Item {
	private:
		char ID[10];//Have different formats per category (e.g., F001, C001, T001, M001)
		int donorID;
		char name[100];
		int amount;
		char category[100];// food, clothing, toy, money
		char description[255];// can put an expiry date if food category
		string dateAdded;//auto generated

	public:
		Item(char* ID, const int& donorID, char* name, const int& amount, char* category, char* description, const string& dateAdded) :
			donorID(donorID), amount(amount), dateAdded(dateAdded) {
			strcpy(this->ID, ID);
			strcpy(this->name, name);
			strcpy(this->category, category);
			strcpy(this->description, description);
		}

		Item() = default;// Default constructor

		bool addItem(User& user) {
			donorID = user.getID();

			cout << "Enter item name: ";
			cin.getline(name, 100);
			while (strlen(name) == 0) {// Validate name not empty
				cout << "Name cannot be empty. Please re-enter: ";
				cin.getline(name, 100);
			}

			cout << "Enter item amount: ";
			cin >> amount;
			while (amount <= 0) {
				cout << "Amount must be greater than 0. Please re-enter: ";
				cin >> amount;
			}

			char catCode;
			cout << "Enter item category" << endl;
			cout << "[F]ood" << endl;
			cout << "[C]lothing" << endl;
			cout << "[T]oy" << endl;
			cout << "[M]oney" << endl;
			cout << "Enter category code: ";
			cin >> catCode;
			toUpperCase(to_string(catCode)); // Convert to lowercase for consistency
			if (catCode != 'F' && catCode != 'C' && catCode != 'T' && catCode != 'M') {
				catCode = 'X';
				strcpy(category, "unknown");
			}

			switch (catCode) {
				case 'F':
					strcpy(category, "Food");
					break;
				case 'C':
					strcpy(category, "Clothing");
					break;
				case 'T':
					strcpy(category, "Toy");
					break;
				case 'M':
					strcpy(category, "Money");
					break;
			}

			if (catCode == 'F') {//Food category must have expiry date
				cout << "Enter expiry date (DD-MM-YYYY): ";
				cin.getline(description, 255);
				while (strlen(description) == 0 || isValidDate(description)) {
					cout << "Invalid expiry date. Please re-enter: ";
					cin.getline(description, 255);
				}
			} else {
				cout << "Enter item description (Leave empty if none): ";
				cin.getline(description, 255);
			}

			dateAdded = currentDateTime();

			string query = "INSERT INTO item (ID, DONORID, NAME, AMOUNT, CATEGORY, DESCRIPTION, DATEADDED) VALUES ('" +
				   string(generateID(catCode)) + "'," +
				   to_string(donorID) + ",'" +
				   string(name) + "'," +
				   to_string(amount) + ",'" +
				   string(description) + "','" +
				   string(category) + "','" +
				   dateAdded + "')";
			return mysql_query(conn, query.c_str()) == 0;
		}

		const char* getID() const {
			return ID;
		}
		void setID(const char* ID) {
			strcpy(this->ID, ID);
		}
		string generateID(char catCode) {
			if (catCode == 'X') {
				return "";
			}

			string query = "SELECT ID FROM item WHERE ID LIKE '" + string(1, catCode) + "%' ORDER BY ID DESC LIMIT 1";
			if (mysql_query(conn, query.c_str()) != 0) {// Error with query
				return "";
			}

			res = mysql_store_result(conn);
			int lastNumber = 0;
			if (res && mysql_num_rows(res) > 0) {
				row = mysql_fetch_row(res);
				if (row && row[0]) {
					// Example: row[0] == "F004" (skip the leading category char)
					string lastId(row[0]);
					if (lastId.length() >= 4) {
						lastNumber = stoi(lastId.substr(1)); // Get numeric part
					}
				}
			}
			if (res) mysql_free_result(res);

			int nextNumber = lastNumber + 1;
			stringstream ss;
			ss << catCode << setw(3) << setfill('0') << nextNumber;
			return ss.str(); // e.g., F005
		}

		int getDonorId() const {
			return donorID;
		}
		void setDonorID(int donorID) {
			this->donorID = donorID;
		}

		const char* getName() const {
			return name;
		}
		void setName(const char* name) {
			strcpy(this->name, name);
		}

		int getAmount() const {
			return amount;
		}
		void setAmount(int amount) {
			this->amount = amount;
		}

		const char* getCategory() const {
			return category;
		}
		void setCategory(char * category) {
			strcpy(this->category, category);
		}

		const char* getDescription() const {
			return description;
		}
		void setDescription(const char* description) {
			strcpy(this->description, description);
		}

		string getDateAdded() const {
			return dateAdded;
		}
		void setDateAdded(const string& dateAdded) {
			this->dateAdded = dateAdded;
		}
};

#endif