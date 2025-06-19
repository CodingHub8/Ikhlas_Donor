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
            int remainingAmount;

            strcpy(ID, generateID().c_str());// generate request ID (e.g. REQ001)
            strcpy(recipientID, user.getID());// copy recipient ID to request ID

            cout << "Please enter the item ID to request: ";
            cin >> itemID;
            strcpy(itemID, toUpperCase(itemID).c_str());

            // item amount validation
            string query = "SELECT AMOUNT FROM item WHERE ID = '" + string(itemID) + "'";
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error fetching item amount: " << mysql_error(conn) << endl;
                return false;
            }
            res = mysql_store_result(conn);
            if (res) {
                row = mysql_fetch_row(res);
                remainingAmount = atoi(row[0]);
                mysql_free_result(res);
            }

            cout << "Please enter the amount to request: ";
            do {
                inputint(reqAmount);
                if (reqAmount <= 0 || reqAmount > remainingAmount) {
                    cout << "Invalid amount. Please try again." << endl;
                    cout << "-> ";
                } else {
                    break;
                }
            }while(true);

            if (user.getAddress() == "NULL") {
                cout << "Please enter your address: ";
                getline(cin, requestAddress);
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

            query = "INSERT INTO request (ID, RECIPIENTID, ITEMID, AMOUNT, REQUESTADDRESS, REQUESTDATE, DESCRIPTION, STATUS) VALUES ('" +
                string(ID) + "', '" + string(recipientID) + "', '" + string(itemID) + "', " + to_string(reqAmount) + ", '" +
                    string(requestAddress) + "', '" + string(requestDate) + "', '" + string(description) + "', '" + string(status) + "')";

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
                    string lastId(row[0]);
                    if (lastId.length() >= 6) { // "REQ" + 3 digits
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
