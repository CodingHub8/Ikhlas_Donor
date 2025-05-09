#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
string encryptDecrypt(const string& data) {//encrypt and decrypt data using XOR Cipher
    string result = data;
    const char key = 'K';

    for (char& c : result) {
        c ^= key;
    }

    return result;
}
#pragma clang diagnostic pop

size_t encrypt(const string& input) {// Hash function
    return hash<string>{}(input);
}

string hidePasswordKeys() {//hide password characters
    string password;

    while (true) {
        char c = _getch();  // Use _getch() from conio.h to get a character without echoing
        if (c == 13)   // Enter key
            break;
        else if (c == 8) {  // Backspace key
            if (!password.empty()) {
                cout << "\b \b";  // Move the cursor back and clear the character
                password.pop_back();
            }
        } else {
            password.push_back(c);
            cout << '*';  // Print an asterisk instead of the actual character
        }
    }

    cout << endl;  // Move to the next line after entering the password
    return password;
}

string toUpperCase(const string& str) {
    string result;
    for (char c : str) {
        result += toupper(c);
    }
    return result;
}

string toLowerCase(const string& str) {
    string result;
    for (char c : str) result += tolower(c);
    return result;
}

const string currentDateTime() {
    time_t now = time(nullptr);
    tm tstruct;
    char buf[20];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", &tstruct);
    return buf;
}

bool isValidDate(const string& date) {// Function to validate date format (DD-MM-YYYY)
    // Define the regular expression for the "DD-MM-YYYY" format
    regex dateFormat("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-\\d{4}$");

    // Check if the date matches the regular expression
    return regex_match(date, dateFormat);
}

bool isValidTime(const string &time) {// Function to validate time format (HH:MM:SS)
    // Define a regular expression for time validation (HH:MM:SS)
    const regex timeRegex("^([01]\\d|2[0-3]):([0-5]\\d):([0-5]\\d)$");
    return regex_match(time, timeRegex);
}

bool isNumeric(const string& str) {// Function to check if a string is numeric
    istringstream iss(str);
    double test;
    iss >> noskipws >> test;  // Try to read a double without skipping whitespaces

    return iss.eof() && !iss.fail();  // Return true if the entire string was consumed and no failure occurred
}

bool isNumeric(char ch) {// Overloaded function for char to check if it is numeric
    return isdigit(ch) != 0;
}

bool isValidPhone(const string &phone) {// Define a regular expression for Malaysian phone number validation
    const regex phoneRegex("^(01[0-9]|011|012|013|014|015|016|017|018|019)[0-9]{7,8}$");
    return regex_match(phone, phoneRegex);
}

bool isValidEmail(const string &email) {// Define a regular expression for a basic email validation
    const regex emailRegex("^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,4}$");

    return regex_match(email, emailRegex);
}

bool isValidAge(int age) {// Define a range for a valid age
    return age >= 1 && age <= 150;
}

bool containSpace(const string &str){
	return find(str.begin(), str.end(), ' ') != str.end();
}

string formatPhone(const string &phone) {
    // Regular expressions to match the different Malaysian phone number formats
    regex regex11("^011(\\d{4})(\\d{4})$");
    regex regexOther("^01([0-9])(\\d{3})(\\d{4})$");
    smatch match;

    string formattedPhone;

    if (regex_match(phone, match, regex11)) {
        // Format for numbers starting with 011
        formattedPhone = "+6011-" + match[1].str() + " " + match[2].str();
    } else if (regex_match(phone, match, regexOther)) {
        // Format for numbers starting with 01X (other than 011)
        formattedPhone = "+601" + match[1].str() + "-" + match[2].str() + " " + match[3].str();
    } else {
        // Return the original number if it doesn't match the expected formats
        formattedPhone = phone;
    }

    return formattedPhone;
}

int input(int &inp){//integer input validation
	while(true){// Check if input is an integer
		if(cin >> inp){// Check for remaining characters in the input buffer
			if(cin.peek() == '\n'){
				cin.ignore(10000,'\n');
				break;// Input is valid, break out of loop
			}
		}
		
		cin.clear();
		cin.ignore(10000, '\n');
		
		cout << "Enter integers only. Please try again: ";
	}
	
	return inp;
}