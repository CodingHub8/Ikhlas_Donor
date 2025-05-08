#ifndef USERS_H
#define USERS_H
#include <iostream>
using namespace std;

class User {
	private:
		int ID;
		char name[100];     // individual or company name
		char email[100];    // unique for login purpose
		char password[255]; // hashed
		char phone[20];
		string address;
		string role;        // 'donor' or 'recipient'
		
	public:
		// Parameterized constructor
		User(const int& ID, const char* name, const char* email, const char* password, const char* phone, const string& address, const string& role) :
		      ID(ID), address(address), role(role) {
		    strcpy(this->name, name);
		    strcpy(this->email, email);
		    strcpy(this->password, password);
		    strcpy(this->phone, phone);
		}

		User() = default;// Default constructor

		bool login() {
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
						setID(atoi(row[0]));
						setName(*&row[1]);
						setEmail(*&row[2]);
						setPhone(*&row[4] != nullptr ? *&row[4] : "N/A");
						setAddress(row[5] != nullptr ? row[5] : "N/A");
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

		void signup() {
			char nameInput[100], emailInput[100], phoneInput[20], passwordInput[255], confirmPassword[255];
		    string addressInput, roleInput;

		    cout << "Enter your full name: ";
		    cin.getline(nameInput, 100);

		    // Validate name not empty
		    while (strlen(nameInput) == 0) {
		        cout << "Name cannot be empty. Please re-enter: ";
		        cin.getline(nameInput, 100);
		    }

		    // Email entry and uniqueness check
		    while (true) {
		        cout << "Enter your email: ";
		        cin.getline(emailInput, 100);

		        if (!isValidEmail(emailInput)) {
		            cout << "Invalid email format. Please try again." << endl;
		            continue;
		        }

		        // Uniqueness check
		        string query = "SELECT * FROM user WHERE EMAIL = '" + string(emailInput) + "'";
		        int q = mysql_query(conn, query.c_str());
		        MYSQL_RES* res = mysql_store_result(conn);
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
		        string query = "SELECT * FROM user WHERE PHONE = '" + phoneStr + "'";
		        int q = mysql_query(conn, query.c_str());
		        MYSQL_RES* res = mysql_store_result(conn);
		        if (!q && res && mysql_num_rows(res) > 0) {
		            cout << "Phone number already registered. Please use another phone number." << endl;
		            mysql_free_result(res);
		            continue;
		        }
		        if (res) mysql_free_result(res);
		        break;
		    }

		    cout << "Enter your address: ";
		    getline(cin, addressInput);

		    // Role: donor/recipient
		    while (true) {
		        cout << "Enter your role [donor/recipient]: ";
		        getline(cin, roleInput);
		        roleInput = toLowerCase(roleInput);
		        if (roleInput == "donor" || roleInput == "recipient") break;
		        cout << "Invalid role. Must be 'donor' or 'recipient'." << endl;
		    }

		    // Hash password before storing
		    size_t hashedPassword = encrypt(passwordInput);
		    string encryptedPassword = to_string(hashedPassword);

		    // Insert into DB
			string phoneValue = string(phoneInput).empty() ? "NULL" : ("'" + string(phoneInput) + "'");
			string addressValue = string(addressInput).empty() ? "NULL" : ("'" + string(addressInput) + "'");

		    string insertQuery = 
		        "INSERT INTO user (NAME, EMAIL, PASSWORD, PHONE, ADDRESS, ROLE) VALUES ('" + 
		        string(nameInput) + "', '" +
		        string(emailInput) + "', '" +
		        encryptedPassword + "', " +
		        phoneValue + ", " +
		        addressValue + ", '" +
		        roleInput + "')";
		    int qstate = mysql_query(conn, insertQuery.c_str());

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
		void setID(int ID) {
			this->ID = ID;
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