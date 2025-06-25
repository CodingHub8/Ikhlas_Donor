#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <string>
using namespace std;

class User {
	private:
		char ID[10]; // Donor = D00X, Recipient = R00X
		char name[100];     // individual or company name
		char email[100];    // unique for login purpose
		char password[255]; // hashed
		char phone[20];
		string address;
		string role;        // 'donor' or 'recipient'
		
	public:
		// Parameterized constructor
		User(const char* ID, const char* name, const char* email, const char* password, const char* phone, const string& address, const string& role) :
		      address(address), role(role) {
			strcpy(this->ID, ID);
		    strcpy(this->name, name);
		    strcpy(this->email, email);
		    strcpy(this->password, password);
		    strcpy(this->phone, phone);
		}

		User() = default;// Default constructor

		bool login() {//Read function
			char emailInput[100];
			cout << "Enter Email: ";
			cin.getline(emailInput, 100, '\n');

			cout << "Enter password: ";
			string passwordInput = hidePasswordKeys();

			// Validate credentials from the database
			size_t hashedPassword = encrypt(passwordInput);
			string query = "SELECT * FROM user WHERE EMAIL = '" + string(emailInput) + "' AND PASSWORD = '" + to_string(hashedPassword) + "'";
			qstate = mysql_query(conn, query.c_str());

			if (!qstate) {
				res = mysql_store_result(conn);
				if (res != nullptr && mysql_num_rows(res) == 1) {
					row = mysql_fetch_row(res);
					if (row && row[0]) {
						setID(row[0]);
						setName(row[1]);
						setEmail(row[2]);
						setPhone(row[4] != nullptr ? row[4] : "NULL");
						setAddress(row[5] != nullptr ? row[5] : "NULL");
						setRole(row[6]);
						mysql_free_result(res);
						return true;
					}
				}
			} else {
				cout << "Error in executing query: " << mysql_error(conn) << endl;
			}

			mysql_free_result(res);
			return false;// Return false indicating failed login
		}

		void signup() {//Create function
			char nameInput[100], emailInput[100], phoneInput[20], passwordInput[255], confirmPassword[255];
		    string addressInput, roleInput;

		    cout << "Enter your full name: ";
		    cin.getline(nameInput, 100, '\n');

		    // Validate name not empty
		    while (strlen(nameInput) == 0) {
		        cout << "Name cannot be empty. Please re-enter: ";
		        cin.getline(nameInput, 100, '\n');
		    }

		    // Email entry and uniqueness check
		    while (true) {
		        cout << "Enter your email: ";
		        cin.getline(emailInput, 100, '\n');

		        if (!isValidEmail(emailInput)) {
		            cout << "Invalid email format. Please try again." << endl;
		            continue;
		        }

		        // Uniqueness check
		        string query = "SELECT * FROM user WHERE EMAIL = '" + string(emailInput) + "'";
		        int q = mysql_query(conn, query.c_str());
		        res = mysql_store_result(conn);
		        if (!q && res && mysql_num_rows(res) > 0) {
		            cout << "Email already registered. Please use another email." << endl;
		            mysql_free_result(res);
		            continue;
		        }
		        if (res) mysql_free_result(res);
		        break;
		    }

		    // Password
		    while (true) {
		        cout << "Enter your password: ";
		        strcpy(passwordInput, hidePasswordKeys().c_str());
		        if (strlen(passwordInput) < 6) {
		            cout << "Password must be at least 6 characters." << endl;
		            continue;
		        }
		        cout << "Confirm your password: ";
		        strcpy(confirmPassword, hidePasswordKeys().c_str());
		        if (strcmp(passwordInput, confirmPassword) != 0) {
		            cout << "Passwords do not match. Please try again." << endl;
		        } else {
		            break;
		        }
		    }

		    // Phone validation and uniqueness check
		    while (true) {
		        cout << "Enter your phone number (leave empty if none): ";
		        cin.getline(phoneInput, 20, '\n');
		        string phoneStr = phoneInput;

		        // Allow empty input
		        if (phoneStr.empty()) {
		            break;
		        }

		        if (!isValidPhone(phoneStr)) {
		            cout << "Invalid phone format. Please try again." << endl;
		            continue;
		        }
		        string query = "SELECT * FROM user WHERE PHONE = '" + phoneStr + "'";
		        int q = mysql_query(conn, query.c_str());
		        res = mysql_store_result(conn);
		        if (!q && res && mysql_num_rows(res) > 0) {
		            cout << "Phone number already registered. Please use another phone number." << endl;
		            mysql_free_result(res);
		            continue;
		        }
		        if (res) mysql_free_result(res);
		        break;
		    }

		    cout << "Enter your address (can be edited later): ";
		    getline(cin, addressInput);

		    // Role: donor/recipient
		    while (true) {
		        cout << "Enter your role [donor/recipient]: ";
		        getline(cin, roleInput);
		        roleInput = toLowerCase(roleInput);
		        if (roleInput == "donor" || roleInput == "recipient") break;
		        cout << "Invalid role. Must be 'donor' or 'recipient'." << endl;
		    }
			char roleCode;
			if (roleInput == "recipient") roleCode = 'R';
			else roleCode = 'D';// Default to donor

		    // Hash password before storing
		    size_t hashedPassword = encrypt(passwordInput);
		    string encryptedPassword = to_string(hashedPassword);

		    // Insert into DB
			string phoneValue = string(phoneInput).empty() ? "NULL" : ("'" + string(phoneInput) + "'");
			string addressValue = string(addressInput).empty() ? "NULL" : ("'" + string(addressInput) + "'");

		    string insertQuery = 
		        "INSERT INTO user (ID, NAME, EMAIL, PASSWORD, PHONE, ADDRESS, ROLE) VALUES ('" +
		        generateID(roleCode) + "', '" +
		        string(nameInput) + "', '" +
		        string(emailInput) + "', '" +
		        encryptedPassword + "', " +
		        phoneValue + ", " +
		        addressValue + ", '" +
		        roleInput + "')";
		    qstate = mysql_query(conn, insertQuery.c_str());

		    if (!qstate) {
		    	saveCredentials("Generated Credentials/admin.txt", false, emailInput, passwordInput);
		        cout << "Sign-up successful! You may now log in." << endl;
		    } else {
		        cout << "Failed to sign up: " << mysql_error(conn) << endl;
		    }
		}

		void viewProfile() {//can be used as search
			string input;
			cout << "Enter User ID or User Name: ";
			getline(cin, input);

			string query = "SELECT * FROM user WHERE ID='" + string(toUpperCase(input)) + "' OR NAME LIKE '%" + string(toCamelCase(input)) + "%'";
			if (mysql_query(conn, query.c_str()) != 0) {
				cout << "Error fetching user profile: " << mysql_error(conn) << endl;
				return;
			}
			res = mysql_store_result(conn);
			if (res) {
				if (mysql_num_rows(res) >= 1) {
					while ((row = mysql_fetch_row(res))) {//can display 1 or multiple users
						setID(row[0]);
						setName(row[1]);
						setEmail(row[2]);
						setPassword(row[3]);
						setPhone(row[4] ? row[4] : "NULL");
						setAddress(row[5] ? row[5] : "NULL");
						setRole(row[6]);
						cout << "User ID: " << ID << endl;
						cout << "Name: " << name << endl;
						cout << "Email: " << email << endl;
						cout << "Password: " << password << " (Encrypted)" << endl;
						cout << "Phone: " << phone << endl;
						cout << "Address: " << address << endl;
						cout << "Role: " << role << endl;
						if (mysql_num_rows(res) > 1) {
							cout << setw(address.length()) << setfill('-') << endl;
						}
						cout << endl;
					}
				}
				mysql_free_result(res);
			} else {
				cout << "No user data found." << endl;
			}
		}

		void editProfile() {// Update function (select specific field)
			while (true) {
				cout << "\n--- Edit Profile ---\n";
				cout << "1. Name (" << name << ")\n";
				cout << "2. Phone (" << phone << ")\n";
				cout << "3. Address (" << address << ")\n";
				cout << "4. Password\n";
				cout << "0. Cancel\n";
				cout << "Select the field you want to update [0-5]: ";
				int choice;
				inputint(choice);
				string updateQuery;
				bool updated = false;

				switch (choice) {
					case 1: { // Name
						char nameInput[100];
						cout << "Enter new name: ";
						cin.getline(nameInput, 100, '\n');
						if (strlen(nameInput) == 0) { cout << "Name cannot be empty.\n"; break; }
						updateQuery = "UPDATE user SET NAME = '" + string(nameInput) + "' WHERE ID = '" + string(ID) + "'";
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) strcpy(name, nameInput);
						break;
					}
					case 2: { // Phone
						char phoneInput[20];
						cout << "Enter new phone (leave empty to set as NULL): ";
						cin.getline(phoneInput, 20, '\n');

						string phoneStr = phoneInput; // For usage below
						if (!phoneStr.empty()) {
							// Check if phone already exists for another user
							string checkPhoneQuery = "SELECT ID FROM user WHERE PHONE = '" + phoneStr + "' AND ID != '" + string(ID) + "'";
							if (mysql_query(conn, checkPhoneQuery.c_str()) == 0) {
								res = mysql_store_result(conn);
								if (res && mysql_num_rows(res) > 0) {
									cout << "This phone number is already in use by another account.\n";
									mysql_free_result(res);
									break;
								}
								if (res) mysql_free_result(res);
							} else {
								cout << "Failed to validate phone: " << mysql_error(conn) << endl;
								break;
							}
						}

						string phoneValue = phoneStr.empty() ? "NULL" : ("'" + phoneStr + "'");
						updateQuery = "UPDATE user SET PHONE = " + phoneValue + " WHERE ID = '" + string(ID) + "'";
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) strcpy(phone, phoneStr.empty() ? "" : phoneInput);
						break;
					}
					case 3: { // Address
						string addressInput;
						cout << "Enter new address (leave empty to set as NULL): ";
						getline(cin, addressInput);
						string addressValue = addressInput.empty() ? "NULL" : ("'" + addressInput + "'");
						updateQuery = "UPDATE user SET ADDRESS = " + addressValue + " WHERE ID = '" + string(ID) + "'";
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) address = addressInput.empty() ? "" : addressInput;
						break;
					}
					case 4: { // Password
						char passwordInput[255], confirmPassword[255];
						while (true) {
							cout << "Enter new password: ";
							strcpy(passwordInput, hidePasswordKeys().c_str());
							if (strlen(passwordInput) < 6) {
								cout << "Password must be at least 6 characters.\n";
								continue;
							}
							cout << "Confirm new password: ";
							strcpy(confirmPassword, hidePasswordKeys().c_str());
							if (strcmp(passwordInput, confirmPassword) != 0) {
								cout << "Passwords do not match. Please try again.\n";
							} else {
								break;
							}
						}
						size_t hashedPassword = encrypt(passwordInput);
						updateQuery = "UPDATE user SET PASSWORD = '" + to_string(hashedPassword) + "' WHERE ID = '" + string(ID) + "'";
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) strcpy(password, passwordInput);
						break;
					}
					case 0:
						cout << "Edit cancelled.\n";
						return;
					default:
						cout << "Invalid choice.\n";
						continue;
				}

				if (updated)
					cout << "Update successful!\n";
				else
					cout << "Update failed: " << mysql_error(conn) << endl;
				break;
			}
		}

		bool deleteProfile() {// Delete function with password confirmation
			cout << "\n--- Delete Profile ---\n";
			cout << "Please confirm your password to delete your account (leave empty to cancel): ";
			string passwordInput = hidePasswordKeys();
			size_t hashedPassword = encrypt(passwordInput);

			// Check password
			string checkQuery = "SELECT PASSWORD FROM user WHERE ID = '" + string(ID) + "'";
			int qstate = mysql_query(conn, checkQuery.c_str());
			res = mysql_store_result(conn);
			bool match = false;
			if (!qstate && res && mysql_num_rows(res) == 1) {
				row = mysql_fetch_row(res);
				if (row && row[0] && to_string(hashedPassword) == string(row[0]))
					match = true;
			}
			if (res) mysql_free_result(res);

			if (match) {
				string deleteQuery = "DELETE FROM user WHERE ID = '" + string(ID) + "'";
				qstate = mysql_query(conn, deleteQuery.c_str());
				if (!qstate) {
					cout << "Profile deleted successfully.\n";
					return true;
				}
				cout << "Failed to delete profile: " << mysql_error(conn) << endl;
			} else {
				cout << "Password incorrect. Account not deleted.\n";
			}
			return false;
		}

		// Getter and Setter
		const char* getID() const {
			return ID;
		}
		void setID(const char* ID) {
			strcpy(this->ID, ID);
		}
		string generateID(char roleCode) {
			string query = "SELECT ID FROM user WHERE ID LIKE '" + string(1, roleCode) + "%' ORDER BY ID DESC LIMIT 1";
			if (mysql_query(conn, query.c_str()) != 0) {// Error with query
				return "";
			}

			res = mysql_store_result(conn);
			int lastNumber = 0;
			if (res && mysql_num_rows(res) > 0) {
				row = mysql_fetch_row(res);
				if (row && row[0]) {// Example: row[0] == "R004" (skip the leading category char)
					string lastId(row[0]);
					if (lastId.length() >= 4) {
						lastNumber = stoi(lastId.substr(1)); // Get numeric part
					}
				}
			}
			if (res) mysql_free_result(res);

			int nextNumber = lastNumber + 1;
			stringstream ss;
			ss << roleCode << setw(3) << setfill('0') << nextNumber;
			return ss.str(); // e.g., R005, D001
		}

		const char* getName() const {
			return name;
		}
		void setName(const char* name) {
			strcpy(this->name, name);
		}

		const char* getEmail() const {
			return email;
		}
		void setEmail(const char* email) {
			strcpy(this->email, email);
		}

		const char* getPassword() const {
			return password;
		}
		void setPassword(const char* password) {
			strcpy(this->password, password);
		}

		const char* getPhone() const {
			return phone;
		}
		void setPhone(const char* phone) {
			strcpy(this->phone, phone);
		}

		string getAddress() const {
			return address;
		}
		void setAddress(const string& address) {
			this->address = address;
		}

		string getRole() const {
			return role;
		}
		void setRole(const string& role) {
			this->role = role;
		}

		~User() = default;// Destructor
};

#endif