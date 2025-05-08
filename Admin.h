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

		bool login() {
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

		void signup() {
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