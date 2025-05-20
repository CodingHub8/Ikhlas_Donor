#ifndef ITEM_H
#define ITEM_H
#include <iostream>
using namespace std;

class Item {
	private:
		char ID[10];//Have different formats per category (e.g., F001, C001, T001, M001)
		char donorID[10];
		char name[100];
		unsigned int amount;
		char category[100];// food, clothing, toy, money
		char description[255];// can put an expiry date if food category
		string dateAdded;//auto generated

	public:
		Item(char* ID, char* donorID, char* name, const int& amount, char* category, char* description, const string& dateAdded) :
		amount(amount), dateAdded(dateAdded) {
			strcpy(this->ID, ID);
			strcpy(this->donorID, donorID);
			strcpy(this->name, name);
			strcpy(this->category, category);
			strcpy(this->description, description);
		}

		Item() = default;// Default constructor

		bool addItem(User& user) {// Create
			strcpy(donorID, user.getID());// Copy donor ID to item ID

			cout << "Enter item name: ";
			cin.getline(name, 100, '\n');
			while (strlen(name) == 0) {// Validate name not empty
				cout << "Name cannot be empty. Please re-enter: ";
				cin.getline(name, 100, '\n');
			}

			cout << "Enter item amount: ";
			cin >> amount;
			while (amount <= 0) {
				cout << "Amount must be greater than 0. Please re-enter: ";
				cin >> amount;
			}

			char catCode;
			cout << "----- Item categories -----" << endl;
			cout << "[F]ood" << endl;
			cout << "[C]lothing" << endl;
			cout << "[T]oy" << endl;
			cout << "[M]oney" << endl;
			cout << "Enter category code: ";
			cin >> catCode;
			catCode = toupper(catCode); // Convert to uppercase for consistency
			if (catCode != 'F' && catCode != 'C' && catCode != 'T' && catCode != 'M') {
				catCode = 'X';
				strcpy(category, "unknown");
			}

			switch (catCode) {
				case 'F': strcpy(category, "Food"); break;
				case 'C': strcpy(category, "Clothing"); break;
				case 'T': strcpy(category, "Toy"); break;
				case 'M': strcpy(category, "Money"); break;
			}

			if (catCode == 'F') {//Food category must have expiry date
				cout << "Enter expiry date (YYYY-MM-DD): ";
				cin.ignore();
				cin.getline(description, 255, '\n');
				while (strlen(description) == 0 || !isValidDate(description) || !isFutureDate(description)) {
                    cout << "Invalid expiry date. Please re-enter: ";
                    cin.getline(description, 255, '\n');
                }
				// Prepend "Best Before: " to the description
				string expiryStr = "Best Before: ";
				expiryStr += description;
				strncpy(description, expiryStr.c_str(), 254);
				description[254] = '\0'; // Ensure null-termination
			} else {
				cout << "Enter item description (Leave empty if none): ";
				cin.ignore();
				cin.getline(description, 255, '\n');
			}

			dateAdded = currentDateTime();

			string query = "INSERT INTO item (ID, DONORID, NAME, AMOUNT, CATEGORY, DESCRIPTION, DATEADDED) VALUES ('" +
				   generateID(catCode) + "', '" +
				   string(donorID) + "', '" +
				   string(name) + "', " +
				   to_string(amount) + ", '" +
				   string(category) + "', '" +
				   string(description) + "', '" +
				   dateAdded + "')";
			return mysql_query(conn, query.c_str()) == 0;
		}

		void viewItem() {// Read
			string input;
			cout << "Enter Item ID or Item Name: ";
			getline(cin, input);

			string query = "SELECT * FROM item WHERE ID='" + string(ID) + "' OR NAME='" + string(name) + "'";
			if (mysql_query(conn, query.c_str()) != 0) {
				cout << "Error fetching item: " << mysql_error(conn) << endl;
				return;
			}
			res = mysql_store_result(conn);
			if (res) {
				row = mysql_fetch_row(res);
				if (row) {
					cout << "Item ID: " << row[0] << endl;
					cout << "Donor ID: " << row[1] << endl;
					cout << "Name: " << row[2] << endl;
					cout << "Amount: " << row[3] << endl;
					cout << "Category: " << row[4] << endl;
					cout << "Description: " << (row[5] ? row[5] : "") << endl;
					cout << "Date Added: " << row[6] << endl;
				} else {
					cout << "Item not found." << endl;
				}
				mysql_free_result(res);
			} else {
				cout << "No item data found." << endl;
			}

		}

		bool editItem(User& user) {// Update
			// Only allows the donor (owner) to edit
			if (strcmp(user.getID(), donorID) != 0) {
				cout << "You are not allowed to edit this item." << endl;
				return false;
			}

			cout << "Editing item: " << ID << endl;

			// Edit name
			char newName[100];
			cout << "Enter new name (currently '" << name << "'): ";
			cin.ignore();
			cin.getline(newName, 100, '\n');
			if (strlen(newName) > 0) strcpy(name, newName);

			// Edit amount
			cout << "Enter new amount (currently " << amount << "): ";
			do {
				unsigned int newAmount;
				cin >> newAmount;
				if (newAmount > 0) {
					amount = newAmount;
					break;
				}
				cout << "Amount must be greater than 0. Please re-enter: ";
			} while (true);
			cin.ignore();

			// Edit description
			if (strcmp(category, "Food") == 0) {
				cout << "Enter new expiry date (YYYY-MM-DD): ";
				char newExpiry[255];
				cin.getline(newExpiry, 255, '\n');
				while (strlen(newExpiry) == 0 || !isValidDate(newExpiry) || !isFutureDate(newExpiry)) {
					cout << "Invalid expiry date. Please re-enter: ";
					cin.getline(newExpiry, 255, '\n');
				}
				string expiryStr = "Best Before: ";
				expiryStr += newExpiry;
				strncpy(description, expiryStr.c_str(), 254);
				description[254] = '\0'; // Ensure null-termination
			} else {
				cout << "Enter new description (currently '" << description << "'): ";
				char newDesc[255];
				cin.getline(newDesc, 255, '\n');
				if (strlen(newDesc) > 0) strcpy(description, newDesc);
			}

			string query = "UPDATE item SET NAME='" + string(name) +
						   "', AMOUNT=" + to_string(amount) +
						   ", DESCRIPTION='" + string(description) +
						   "' WHERE ID='" + string(ID) + "'";
			if (mysql_query(conn, query.c_str()) == 0) {
				return true;
			}

			return false;
		}

		bool deleteItem(User& user) {// Delete
			// Only allows the donor (owner) to delete
			if (strcmp(user.getID(), donorID) != 0) {
				cout << "You are not allowed to delete this item." << endl;
				return false;
			}

			cout << "Enter item ID to delete: ";
			cin.getline(ID, 10, '\n');

			string query = "DELETE FROM item WHERE ID='" + string(ID) + "'";
			if (mysql_query(conn, query.c_str()) == 0) {
				return true;
			}
			return false;
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
				if (row && row[0]) {// Example: row[0] == "F004" (skip the leading category char)
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

		const char* getDonorId() const {
			return donorID;
		}
		void setDonorID(const char* donorID) {
			strcpy(this->donorID, donorID);
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
		void setCategory(const char* category) {
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