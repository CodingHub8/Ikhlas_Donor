#define NOMINMAX             // Disables min/max macros from Windows
#define WIN32_LEAN_AND_MEAN  // Reduces Windows header bloat
#include <windows.h>         // If you use Windows APIs
#undef byte                  // Undefine any existing 'byte' macro
#include <cstddef>           // For std::byte
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

const string currentDateTime() {// YYYY-MM-DD HH:MM:SS format
    time_t now = time(nullptr);
    tm tstruct;
    char buf[20]; // Enough for "YYYY-MM-DD HH:MM:SS" + null terminator
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return string(buf);
}

bool isFutureDate(const string& date) {
    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) return false;

    time_t t = time(nullptr);
    tm* now = localtime(&t);
    if (!now) return false;

    // Convert today to YYYY-MM-DD
    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    if (year > currYear) return true;
    if (year == currYear && month > currMonth) return true;
    if (year == currYear && month == currMonth && day > currDay) return true;
    return false;
}

bool isValidDate(const string& date) {// YYYY-MM-DD format
    // Check format
    regex dateFormat("^(\\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");

    smatch match;
    if (!regex_match(date, match, dateFormat)) {
        return false;
    }

    // Extract day, month, year
    int day, month, year;
    sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

    // Days in month
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Leap year check
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && isLeap) {
        daysInMonth[1] = 29;
    }

    // Check actual day range
    return day <= daysInMonth[month - 1];
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

int inputint(int &inp){//integer input validation
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