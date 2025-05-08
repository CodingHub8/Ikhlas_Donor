#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
using namespace std;

class Admin {
	private:
		int ID;
		char username[100];
		char email[100];    // unique for login purpose
		char password[255]; // hashed
		char phone[20];

	public:
		// Parameterized constructor
		Admin(const int& ID, const char* username, const char* email, const char* password, const char* phone) :
		      ID(ID) {
		    strcpy(this->username, username);
		    strcpy(this->email, email);
		    strcpy(this->password, password);
		    strcpy(this->phone, phone);
		}

		Admin() = default;// Default constructor

		bool login() {//Read function
			char input[100];
			cout << "Enter Username or Email: ";
			cin.getline(input, 100, '\n');

			cout << "Enter password: ";
			string passwordInput = hidePasswordKeys();

			// Validate credentials from the database
			size_t hashedPassword = encrypt(passwordInput);
			string query = "SELECT * FROM admin WHERE (USERNAME = '" + string(input) + "' OR EMAIL = '" + string(input) + "') AND PASSWORD = '" + to_string(hashedPassword) + "'";
			qstate = mysql_query(conn, query.c_str());

			if (!qstate) {
				res = mysql_store_result(conn);
				if (res != nullptr && mysql_num_rows(res) == 1) {
					row = mysql_fetch_row(res);
					if (row && row[0]) {
						setID(atoi(row[0]));
						setUsername(*&row[1]);
						setEmail(*&row[2]);
						setPhone(*&row[4] != nullptr ? *&row[4] : "N/A");
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

			// Check if there is already an admin account
			string checkAdminQuery = "SELECT COUNT(*) FROM admin";
			qstate = mysql_query(conn, checkAdminQuery.c_str());
			res = mysql_store_result(conn);

			if (qstate == 0 && res) {
			    row = mysql_fetch_row(res);
			    if (row && atoi(row[0]) >= 2) {
			        cout << "The total of existing admin accounts cannot exceed its limit (2)." << endl;
			        mysql_free_result(res);
			        return;
			    }
			    mysql_free_result(res);
			}

			while (true) {// Uniqueness check for username
				cout << "Enter your username: ";
				cin.getline(nameInput, 100);

				// Validate name not empty
				if (strlen(nameInput) == 0) {
					cout << "Username cannot be empty. Please re-enter: ";
					continue;
				}

				// Uniqueness check for username
				string usernameQuery = "SELECT * FROM admin WHERE USERNAME = '" + string(nameInput) + "'";
				qstate = mysql_query(conn, usernameQuery.c_str());
				res = mysql_store_result(conn);
				if (!qstate && res && mysql_num_rows(res) > 0) {
					cout << "Username already taken. Please choose another username." << endl;
					mysql_free_result(res);
					continue;
				}
				if (res) mysql_free_result(res);
				break;
			}

		    while (true) {// Email entry and uniqueness check
		        cout << "Enter your email: ";
		        cin.getline(emailInput, 100);

		        if (!isValidEmail(emailInput)) {
		            cout << "Invalid email format. Please try again." << endl;
		            continue;
		        }

		        // Uniqueness check
		        string query = "SELECT * FROM admin WHERE EMAIL = '" + string(emailInput) + "'";
		        qstate = mysql_query(conn, query.c_str());
		        res = mysql_store_result(conn);
		        if (!qstate && res && mysql_num_rows(res) > 0) {
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
		        if (strlen(passwordInput) < 8) {
		            cout << "Password must be at least 8 characters." << endl;
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

		    while (true) {// Phone validation and uniqueness check
		        cout << "Enter your phone number (leave empty if none): ";
		        cin.getline(phoneInput, 20);
		        string phoneStr = phoneInput;

		        // Allow empty input
		        if (phoneStr.empty()) {
		            break;
		        }

		        if (!isValidPhone(phoneStr)) {
		            cout << "Invalid phone format. Please try again." << endl;
		            continue;
		        }
		        string query = "SELECT * FROM admin WHERE PHONE = '" + phoneStr + "'";
		        qstate = mysql_query(conn, query.c_str());
		        res = mysql_store_result(conn);
		        if (!qstate && res && mysql_num_rows(res) > 0) {
		            cout << "Phone number already registered. Please use another phone number." << endl;
		            mysql_free_result(res);
		            continue;
		        }
		        if (res) mysql_free_result(res);
		        break;
		    }

		    // Hash password before storing
		    size_t hashedPassword = encrypt(passwordInput);
		    string encryptedPassword = to_string(hashedPassword);

			string phoneValue = string(phoneInput).empty() ? "NULL" : ("'" + string(phoneInput) + "'");

			// Insert into DB
		    string insertQuery =
		        "INSERT INTO admin (USERNAME, EMAIL, PASSWORD, PHONE) VALUES ('" +
		        string(nameInput) + "', '" +
		        string(emailInput) + "', '" +
		        encryptedPassword + "', " +
		        phoneValue + ")";
		    qstate = mysql_query(conn, insertQuery.c_str());

		    if (!qstate) {
		        cout << "Sign-up successful! You may now log in." << endl;
		    } else {
		        cout << "Failed to sign up: " << mysql_error(conn) << endl;
		    }
		}

		void editProfile() {// Update function (select specific field)
			while (true) {
				cout << "\n--- Edit Profile ---\n";
				cout << "1. Password\n";
				cout << "2. Phone (" << phone << ")\n";
				cout << "0. Cancel\n";
				cout << "Select the field you want to update [0-2]: ";
				int choice;
				input(choice);
				string updateQuery;
				bool updated = false;

				switch (choice) {
					case 1: { // Password
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
						updateQuery = "UPDATE admin SET PASSWORD = '" + to_string(hashedPassword) + "' WHERE ID = " + to_string(ID);
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) strcpy(password, passwordInput);
						break;
					}
					case 2: { // Phone
						char phoneInput[20];
						cout << "Enter new phone (leave empty to set as NULL): ";
						cin.getline(phoneInput, 20);

						string phoneStr = phoneInput; // For usage below
						if (!phoneStr.empty()) {
							// Check if phone already exists for another admin
							string checkPhoneQuery = "SELECT ID FROM admin WHERE PHONE = '" + phoneStr + "' AND ID != " + to_string(ID);
							if (mysql_query(conn, checkPhoneQuery.c_str()) == 0) {
								MYSQL_RES* res = mysql_store_result(conn);
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
						updateQuery = "UPDATE admin SET PHONE = " + phoneValue + " WHERE ID = " + to_string(ID);
						updated = (mysql_query(conn, updateQuery.c_str()) == 0);
						if (updated) strcpy(phone, phoneStr.empty() ? "" : phoneInput);
						break;
					}
					case 6:
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
			cout << "\n--- Delete Admin Profile ---\n";
			cout << "Please confirm your password to delete your account: ";
			string passwordInput = hidePasswordKeys();
			size_t hashedPassword = encrypt(passwordInput);

			// Check password
			string checkQuery = "SELECT PASSWORD FROM admin WHERE ID = " + to_string(ID);
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
				string deleteQuery = "DELETE FROM admin WHERE ID = " + to_string(ID);
				qstate = mysql_query(conn, deleteQuery.c_str());
				if (!qstate) {
					cout << "Profile deleted successfully.\n";
					return true;
				}
				cout << "Failed to delete profile: " << mysql_error(conn) << endl;
			} else {
				cout << "Password incorrect. Failed to delete profile.\n";
			}
			return false;
		}

		// Getter and Setter
		int getID() const {
			return ID;
		}
		void setID(int const ID) {
			this->ID = ID;
		}

		const char* getUsername() const {
			return username;
		}
		void setUsername(const char* username) {
			strcpy(this->username, username);
		}

		const char* getEmail() const {
			return email;
		}
		void setEmail(const char* email) {
			strcpy(this->email, email);
		}

		const char* getPhone() const {
			return phone;
		}
		void setPhone(const char* phone) {
			strcpy(this->phone, phone);
		}

		~Admin() = default;// Destructor
};

#endif //ADMIN_H