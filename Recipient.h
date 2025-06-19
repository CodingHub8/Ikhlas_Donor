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
        string status;
    public:
        // Parameterized constructor
        Request(const char* ID, const char* recipientID, const char* itemID, const int& amount, const string& requestAddress, const string& requestDate, const string& status) :
               amount(amount), requestAddress(requestAddress), requestDate(requestDate), status(status){
            strcpy(this->ID, ID);
            strcpy(this->recipientID, recipientID);
            strcpy(this->itemID, itemID);
        }

        Request() = default;// Default constructor

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

        string getStatus() const {
            return status;
        }

        void setStatus(const string& status){
            this->status = status;
        }
};

#endif //RECIPIENT_H
