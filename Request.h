#ifndef RECIPIENT_H
#define RECIPIENT_H
#include <iostream>
#include <string>
using namespace std;

class Request {
    private:
        char ID[10];
        char recipientID[10];
        char itemID[10];
        int amount;
        string requestAddress;
        string requestDate;
        string description;
        string status;
    public:
        // Parameterized constructor
        Request(const char* ID, const char* recipientID, const char* itemID, const int& amount, const string& requestAddress, const string& requestDate, const string& description, const string& status) :
               amount(amount), requestAddress(requestAddress), requestDate(requestDate), description(description), status(status){
            strcpy(this->ID, ID);
            strcpy(this->recipientID, recipientID);
            strcpy(this->itemID, itemID);
        }

        Request() = default;// Default constructor

        bool createRequest(User& user) {
            int reqAmount;
            //int remainingAmount = 0;

            strcpy(ID, generateID().c_str());// generate request ID (e.g. REQ001)
            strcpy(recipientID, user.getID());// copy recipient ID to request ID

            cout << "Please enter the item ID to request (Enter '0' to cancel): ";
            cin >> itemID;
            if (strcmp(itemID, "0") == 0) {
                return false;
            }
            strcpy(itemID, toUpperCase(itemID).c_str());

            // item amount validation
            // string query = "SELECT AMOUNT FROM item WHERE ID = '" + string(itemID) + "'";
            // if (mysql_query(conn, query.c_str()) != 0) {
            //     cout << "Error fetching item amount: " << mysql_error(conn) << endl;
            //     return false;
            // }
            // res = mysql_store_result(conn);
            // if (res) {
            //     row = mysql_fetch_row(res);
            //     remainingAmount = atoi(row[0]);
            //     mysql_free_result(res);
            // }

            cout << "Please enter the amount to request: ";
            do {
                inputint(reqAmount);
                if (reqAmount < 1 /*|| reqAmount > remainingAmount*/) {
                    cout << "Invalid amount. Please try again." << endl;
                    cout << "-> ";
                } else {
                    break;
                }
            }while(true);

            if (user.getAddress() == "NULL") {//Validation to check if user already got address during registration or not
                do {
                    cout << "Please enter your address: ";
                    getline(cin, requestAddress);
                }while(requestAddress.empty());
            } else {
                cout << "Please enter the request address (leave empty to use default address): ";
                getline(cin, requestAddress);
                if (requestAddress.empty()) {
                    requestAddress = user.getAddress();
                }
            }

            requestDate = currentDateTime();

            cout << "Enter request description (optional): ";
            getline(cin, description);

            status = "pending";

            string query = "INSERT INTO request (ID, RECIPIENTID, ITEMID, AMOUNT, REQUESTADDRESS, REQUESTDATE, DESCRIPTION, STATUS) VALUES ('" +
                string(ID) + "', '" + string(recipientID) + "', '" + string(itemID) + "', " + to_string(reqAmount) + ", '" +
                    string(requestAddress) + "', '" + string(requestDate) + "', '" + string(description) + "', '" + string(status) + "')";

            if (mysql_query(conn, query.c_str()) == 0) {
                return true;
            }

            return false;
        }

        void viewRequest() {// Read (can also be used for searching)
            string input;
            cout << "Enter Request ID: ";
            getline(cin, input);

            string query = "SELECT * FROM request WHERE ID='" + string(toUpperCase(input)) + "'";
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error fetching request data: " << mysql_error(conn) << endl;
                return;
            }
            res = mysql_store_result(conn);
            if (res) {
                row = mysql_fetch_row(res);
                if (row) {
                    strcpy(ID, row[0]);
                    strcpy(recipientID, row[1]);
                    strcpy(itemID, row[2]);
                    setAmount(atoi(row[3]));
                    setRequestAddress(row[4]);
                    setRequestDate(row[5]);
                    setDescription(row[6] ? row[6] : "NULL");
                    setStatus(row[7]);

                    cout << "Request ID: " << ID << endl;
                    cout << "Recipient ID: " << recipientID << endl;
                    cout << "Item ID: " << itemID << endl;
                    cout << "Amount: " << amount << endl;
                    cout << "Request Address: " << requestAddress << endl;
                    cout << "Request Date: " << requestDate << endl;
                    cout << "Description: " << description << endl;
                    cout << "Status: " << status << endl;
                    return;
                }
                cout << "Request not found." << endl;
                mysql_free_result(res);
            } else {
                cout << "No request data found." << endl;
            }
        }

        void viewAllRequests(const string& query) {//Admin usage
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
                     << setfill(' ') << setw(10) << "Status" << + " |"//status
                     << endl;
                cout << setw(191) << setfill('-') << "" << endl;
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
                cout << setw(191) << setfill('-') << "" << endl;
                cout << endl;
                mysql_free_result(res);
            }
        }

        bool verifyUser(User& user, const string &query) {
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error fetching request owner: " << mysql_error(conn) << endl;
                return false;
            }

            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            if (strcmp(user.getID(), row[0]) != 0) {
                cout << "You are not allowed to modify any request(s) that you did not make." << endl;
                mysql_free_result(res);
                return false;
            }
            mysql_free_result(res);
            return true;
        }

        bool cancelRequest(User& user) {// Delete
            // Retrieve fresh RECIPIENTID from DB for checking permission
            string query = "SELECT RECIPIENTID FROM request WHERE RECIPIENTID='" + string(user.getID()) + "'";
            if (!verifyUser(user, query)) {
                return false;
            }

            cout << "Enter request ID to cancel (Enter '0' to revert cancellation): ";
            cin.getline(ID, 10, '\n');
            if (strcmp(ID, "0") == 0) {
                return false; // Cancel deletion
            }

            query = "SELECT * FROM request WHERE ID='" + string(ID) + "' AND RECIPIENTID='" + string(user.getID()) + "' AND STATUS='pending'";
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error fetching request data: " << mysql_error(conn) << endl;
                return false;
            }
            res = mysql_store_result(conn);
            if (res) {
                if (mysql_num_rows(res) == 0) {
                    cout << "Enter only the request ID listed above." << endl;
                    return false;
                }
            }

            //recipient can only cancel request with 'pending' status
            query = "DELETE FROM request WHERE ID='" + string(ID) + "' AND RECIPIENTID='" + string(user.getID()) + "' AND STATUS='pending'";
            if (mysql_query(conn, query.c_str()) == 0) {
                return true;
            }

            return false;
        }

        string generateID() {
            string query = "SELECT ID FROM request WHERE ID LIKE 'REQ%' ORDER BY ID DESC LIMIT 1";
            if (mysql_query(conn, query.c_str()) != 0) { // Error with query
                return "";
            }

            res = mysql_store_result(conn);
            int lastNumber = 0;
            if (res && mysql_num_rows(res) > 0) {
                row = mysql_fetch_row(res);
                if (row && row[0]) {
                    if (string lastId(row[0]); lastId.length() >= 6) { // "REQ" + 3 digits
                        lastNumber = stoi(lastId.substr(3)); // Numeric part after "REQ"
                    }
                }
            }
            if (res) mysql_free_result(res);

            int nextNumber = lastNumber + 1;
            stringstream ss;
            ss << "REQ" << setw(3) << setfill('0') << nextNumber;
            return ss.str(); // e.g., REQ005
        }

        // Getter & Setter
        const char* getID() const {
            return ID;
        }

        void setID(const char* ID) {
            strcpy(this->ID, ID);
        }

        const char* getRecipientID() const {
            return recipientID;
        }

        void setRecipientID(const char* recipientID) {
            strcpy(this->recipientID, recipientID);
        }

        const char* getItemID() const {
            return itemID;
        }

        void setItemID(const char* itemID) {
            strcpy(this->itemID, itemID);
        }

        int getAmount() const {
            return amount;
        }

        void setAmount(int amount){
            this->amount = amount;
        }

        string getRequestAddress() const {
            return requestAddress;
        }

        void setRequestAddress(const string &requestAddress){
            this->requestAddress = requestAddress;
        }

        string getRequestDate() const {
            return requestDate;
        }

        void setRequestDate(const string &requestDate){
            this->requestDate = requestDate;
        }

        string getDescription() const {
            return description;
        }

        void setDescription(const string &description) {
            this->description = description;
        }

        string getStatus() const {
            return status;
        }

        void setStatus(const string& status){
            this->status = status;
        }
};

#endif //RECIPIENT_H
