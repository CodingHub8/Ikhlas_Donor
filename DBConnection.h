#include <mysql.h>
#include <mysqld_error.h>

#include "faker-cxx/commerce.h"
#include "faker-cxx/Person.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/Phone.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/food.h"
#include "faker-cxx/location.h"
#include "faker-cxx/Word.h"
#include "faker-cxx/Number.h"
#include "faker-cxx/String.h"

#include <fstream>
#include <filesystem>

#include "faker-cxx/company.h"
#include "faker-cxx/lorem.h"
#include "faker-cxx/sport.h"
using namespace std;

/*
    Faker library cloned from https://github.com/cieslarmichal/faker-cxx.git
*/

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
MYSQL_STMT* stmt;

struct Column {
    string name;
    string type;   // e.g., "VARCHAR", "INT"
    int size;      // 0 if not used (e.g., INT), >0 for VARCHAR, etc.
    string extras; // For things like "PRIMARY KEY", "NOT NULL", "UNIQUE"
};

void saveCredentials(const string& filepath, bool overwrite, const string& username, const string& password){
    // Extract the parent directory path
    filesystem::path pathObj(filepath);
    if (pathObj.has_parent_path()) {
        filesystem::create_directories(pathObj.parent_path());
    }

    ios_base::openmode mode = ios::out;
    if (!overwrite) {
        mode |= ios::app;
    }

    ofstream file(filepath, mode);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filepath);
    }

    file << left << setw(50) << setfill(' ') << username << ", " << password << endl;
}


class DBConnection {
    public:
        static void ConnectionFunction() {
            conn = mysql_init(0);
            if (!conn) {
                cout << "Failed To Connect!" << endl;
                exit(1); // Exit if connection fails
            }

            conn = mysql_real_connect(conn, "localhost", "root", "", "ikhlasdb", 3306, nullptr, 0);
            if (conn)
                cout << "Connected to database: " << conn->db << endl << endl;
            else{
                cout << "Failed To Connect!" << endl;
                exit(1); // Exit if the connection fails
            }
        }

        static void PopulateDatabase() {
            // Counters for ID generation
            int donorCount = 1;
            int recipientCount = 1;
            map<string, int> itemCounters = {
                {"Food", 1},
                {"Clothing", 1},
                {"Toy", 1},
                {"Money", 1}
            };
            int rowCount = 0;
            string countQuery;

            // Generate fake users (both donors and recipients) only when 'user' rows are less than 10
            countQuery = "SELECT COUNT(*) FROM user";
            qstate = mysql_query(conn, countQuery.c_str());
            if (qstate == 0) {
                res = mysql_store_result(conn);
                if ((row = mysql_fetch_row(res)) != nullptr) {
                    rowCount = atoi(row[0]);
                    if (rowCount < 20) {
                        for (int i = 0; i < 20; i++) {
                            string id;
                            string role;

                            if (i % 3 == 0) { // 1/3 donors
                                role = "donor";
                                id = "D" + string(3 - to_string(donorCount).length(), '0') + to_string(donorCount);
                                donorCount++;
                            } else { // 2/3 recipients
                                role = "recipient";
                                id = "R" + string(3 - to_string(recipientCount).length(), '0') + to_string(recipientCount);
                                recipientCount++;
                            }

                            const string name = faker::person::fullName();
                            const string email = faker::internet::email();
                            const string password = faker::internet::password();
                            const string phone = faker::phone::phoneNumberByCountry(faker::phone::PhoneNumberCountryFormat::Malaysia);
                            const string address = faker::location::streetAddress() + ", " + faker::location::city();

                            string query = "INSERT INTO user (ID, NAME, EMAIL, PASSWORD, PHONE, ADDRESS, ROLE) VALUES ('" +
                                           id + "', '" + name + "', '" + email + "', '" + to_string(encrypt(password)) + "', '" + phone + "', '" +
                                           address + "', '" + role + "');";

                            qstate = mysql_query(conn, query.c_str());
                            if (qstate != 0) {
                                cout << "Error inserting user: " << mysql_error(conn) << endl;
                            } else {
                                saveCredentials("Generated Credentials/user.txt", false, email, password);
                            }
                        }
                    }
                }
            }

            // Generate fake admins only when 'admin' rows are less than 3
            countQuery = "SELECT COUNT(*) FROM admin";
            qstate = mysql_query(conn, countQuery.c_str());
            rowCount = 0;
            if (qstate == 0) {
                res = mysql_store_result(conn);
                if ((row = mysql_fetch_row(res)) != nullptr) {
                    rowCount = atoi(row[0]);
                    if (rowCount < 3) {
                        for (int i = 0; i < 3; i++) {
                            const string username = faker::internet::username();
                            const string email = faker::internet::email();
                            const string password = faker::internet::password();
                            const string phone = faker::phone::phoneNumberByCountry(faker::phone::PhoneNumberCountryFormat::Malaysia);

                            string query = "INSERT INTO admin (USERNAME, EMAIL, PASSWORD, PHONE) VALUES ('" +
                                           username + "', '" + email + "', '" + to_string(encrypt(password)) + "', '" + phone + "');";

                            qstate = mysql_query(conn, query.c_str());
                            if (qstate != 0) {
                                cout << "Error inserting admin: " << mysql_error(conn) << endl;
                            } else {
                                saveCredentials("Generated Credentials/admin.txt", false, username, password);
                            }
                        }
                    }
                }
            }

            // Generate fake items (only from donors) only when 'item' rows are less than 10
            countQuery = "SELECT COUNT(*) FROM item";
            qstate = mysql_query(conn, countQuery.c_str());
            rowCount = 0;
            if (qstate == 0) {
                res = mysql_store_result(conn);
                if ((row = mysql_fetch_row(res)) != nullptr) {
                    rowCount = atoi(row[0]);
                    if (rowCount < 30) {
                        vector<string> donorIds;
                        string donorQuery = "SELECT ID FROM user WHERE ROLE = 'donor';";
                        qstate = mysql_query(conn, donorQuery.c_str());
                        if (qstate == 0) {
                            res = mysql_store_result(conn);
                            while ((row = mysql_fetch_row(res))) {
                                donorIds.push_back(row[0]);
                            }
                            mysql_free_result(res);
                        }

                        vector<string> categories = {"Food", "Clothing", "Toy", "Money"};
                        for (int i = 0; i < 30; i++) {
                            if (donorIds.empty()) break;

                            string category = categories[faker::number::integer<size_t>(0, 3)];
                            string idPrefix;
                            string name;
                            string description;
                            int amount = faker::number::integer<int>(10, 1000);

                            switch (category[0]) {
                                case 'F':
                                    idPrefix = "F";
                                    switch(faker::number::integer<int>(0, 11)) {
                                        case 0: name = faker::food::dishName(); break;
                                        case 1: name = faker::food::fruit(); break;
                                        case 2: name = faker::food::grain(); break;
                                        case 3: name = faker::food::meat(); break;
                                        case 4: name = faker::food::milkProduct(); break;
                                        case 5: name = faker::food::nonalcoholicBeverage(); break;
                                        case 6: name = faker::food::nut(); break;
                                        case 7: name = faker::food::oil(); break;
                                        case 8: name = faker::food::seafood(); break;
                                        case 9: name = faker::food::seed(); break;
                                        case 10: name = faker::food::sugarProduct(); break;
                                        case 11: name = faker::food::vegetable(); break;
                                    }
                                    // Generate expiry date (today + random days 1-30)
                                    {
                                        auto expiryDate = chrono::system_clock::now() + chrono::hours(24 * faker::number::integer<int>(1, 30));
                                        time_t expiry_time = chrono::system_clock::to_time_t(expiryDate);
                                        tm expiry_tm = *localtime(&expiry_time);
                                        stringstream dateStream;
                                        dateStream << "Best Before: " << put_time(&expiry_tm, "%Y-%m-%d");
                                        description = dateStream.str();
                                    }
                                    break;
                                case 'C':
                                    idPrefix = "C";
                                    name = faker::company::catchPhraseAdjective(); // or specific clothing items
                                    description = faker::company::catchPhraseDescriptor();
                                    break;
                                case 'T':
                                    idPrefix = "T";
                                    name = faker::sport::sportName(); // or specific toy names
                                    description = faker::sport::sportEvent();
                                    break;
                                case 'M':
                                    idPrefix = "M";
                                    name = "Monetary Donation";
                                    description = "RM " + to_string(amount) + " donation";
                                    break;
                            }

                            string id = idPrefix + string(3 - to_string(itemCounters[category]).length(), '0') + to_string(itemCounters[category]);
                            itemCounters[category]++;

                            string donorId = donorIds[faker::number::integer<size_t>(0, donorIds.size() - 1)];

                            // Generate current datetime
                            auto now = chrono::system_clock::now();
                            time_t now_time = chrono::system_clock::to_time_t(now);
                            tm now_tm = *localtime(&now_time);
                            stringstream dateStream;
                            dateStream << put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
                            string dateAdded = dateStream.str();

                            string query = "INSERT INTO item (ID, DONORID, NAME, AMOUNT, CATEGORY, DESCRIPTION, DATEADDED) VALUES ('" +
                                           id + "', '" + donorId + "', '" + name + "', " + to_string(amount) + ", '" +
                                           category + "', '" + description + "', '" + dateAdded + "');";

                            qstate = mysql_query(conn, query.c_str());
                            if (qstate != 0) {
                                cout << "Error inserting item: " << mysql_error(conn) << endl;
                            }
                        }
                    }
                }
            }

            // Generate fake requests (only from recipients to existing items) only when 'request' rows are less than 10
            countQuery = "SELECT COUNT(*) FROM request";
            qstate = mysql_query(conn, countQuery.c_str());
            rowCount = 0;
            if (qstate == 0) {
                res = mysql_store_result(conn);
                if ((row = mysql_fetch_row(res)) != nullptr) {
                    rowCount = atoi(row[0]);
                    if (rowCount < 50) {
                        vector<string> recipientIds;
                        string recipientQuery = "SELECT ID FROM user WHERE ROLE = 'recipient';";
                        qstate = mysql_query(conn, recipientQuery.c_str());
                        if (qstate == 0) {
                            res = mysql_store_result(conn);
                            while ((row = mysql_fetch_row(res))) {
                                recipientIds.push_back(row[0]);
                            }
                            mysql_free_result(res);
                        }

                        vector<string> itemIds;
                        string itemQuery = "SELECT ID FROM item;";
                        qstate = mysql_query(conn, itemQuery.c_str());
                        if (qstate == 0) {
                            res = mysql_store_result(conn);
                            while ((row = mysql_fetch_row(res))) {
                                itemIds.push_back(row[0]);
                            }
                            mysql_free_result(res);
                        }

                        for (int i = 0; i < 50; i++) {
                            if (recipientIds.empty() || itemIds.empty()) break;

                            string id = "REQ" + string(3 - to_string(i+1).length(), '0') + to_string(i+1);
                            string recipientId = recipientIds[faker::number::integer<size_t>(0, recipientIds.size() - 1)];
                            string itemId = itemIds[faker::number::integer<size_t>(0, itemIds.size() - 1)];
                            int amount = faker::number::integer<>(1, 10);
                            string requestAddress = faker::location::streetAddress() + ", " + faker::location::city();

                            // Generate random date within last 30 days
                            auto now = chrono::system_clock::now();
                            auto past = now - chrono::hours(24 * 30);

                            // Convert to time_t
                            time_t past_time = chrono::system_clock::to_time_t(past);
                            time_t now_time = chrono::system_clock::to_time_t(now);

                            // Generate random timestamp between these two
                            time_t random_time = faker::number::integer<int64_t>(past_time, now_time);

                            // Convert back to tm
                            tm request_tm = *localtime(&random_time);
                            stringstream dateStream;
                            dateStream << put_time(&request_tm, "%Y-%m-%d %H:%M:%S");
                            string requestDate = dateStream.str();

                            string description = faker::lorem::sentences(0, 2);// generate between 0 to 2 sentences

                            string status;
                            switch (int statusRand = faker::number::integer<>(0, 2)) {
                                case 0: status = "pending"; break;
                                case 1: status = "failed"; break;
                                case 2: status = "approved"; break;
                            }

                            string query = "INSERT INTO request (ID, RECIPIENTID, ITEMID, AMOUNT, REQUESTADDRESS, REQUESTDATE, DESCRIPTION, STATUS) VALUES ('" +
                                           id + "', '" + recipientId + "', '" + itemId + "', " + to_string(amount) + ", '" +
                                           requestAddress + "', '" + requestDate + "', " +
                                           (description.empty() ? "NULL" : ("'" + description + "'")) + ", '" + status + "');";

                            qstate = mysql_query(conn, query.c_str());
                            if (qstate != 0) {
                                cout << "Error inserting request: " << mysql_error(conn) << endl;
                            }
                        }
                    }
                }
            }
        }

        static void InitializeDatabase() {
            ConnectionFunction();
            qstate = mysql_query(conn, "CREATE DATABASE IF NOT EXISTS ikhlasdb;");
            if (qstate != 0) {
                cout << "Error creating database: " << mysql_error(conn) << endl;
                exit(1);
            }

            // Select the database
            qstate = mysql_select_db(conn, "ikhlasdb");
            if (qstate != 0) {
                cout << "Error selecting database: " << mysql_error(conn) << endl;
                exit(1);
            }

            vector<Column> userTable = {
                {"ID", "VARCHAR", 10, "PRIMARY KEY"},
                {"NAME", "VARCHAR", 100, "NOT NULL"},
                {"EMAIL", "VARCHAR", 100, "UNIQUE NOT NULL"},
                {"PASSWORD", "VARCHAR", 255, "NOT NULL"},
                {"PHONE", "VARCHAR", 20, "UNIQUE"},
                {"ADDRESS", "TEXT", 0, "DEFAULT NULL"},
                {"ROLE", "ENUM('donor', 'recipient')", 0, "NOT NULL DEFAULT 'recipient'"}
            };

            createTable("user", userTable);

            vector<Column> adminTable = {
                {"ID", "INT", 10, "PRIMARY KEY AUTO_INCREMENT"},
                {"USERNAME", "VARCHAR", 100, "UNIQUE NOT NULL"},
                {"EMAIL", "VARCHAR", 100, "UNIQUE NOT NULL"},
                {"PASSWORD", "VARCHAR", 255, "NOT NULL"},
                {"PHONE", "VARCHAR", 20, "UNIQUE"}
            };

            createTable("admin", adminTable);

            vector<Column> itemTable = {
                {"ID", "VARCHAR", 10, "PRIMARY KEY"},
                {"DONORID", "VARCHAR", 10, "NOT NULL"},
                {"NAME", "VARCHAR", 100, "NOT NULL"},
                {"AMOUNT", "INT", 15, "NOT NULL DEFAULT 1"},
                {"CATEGORY", "ENUM('Food', 'Clothing', 'Toy', 'Money')", 0, "DEFAULT NULL"},
                {"DESCRIPTION", "TEXT", 0, "DEFAULT NULL"},
                {"DATEADDED", "DATETIME", 0, "NOT NULL"}
            };
            vector<string> itemForeignKeys = {
                "FOREIGN KEY (DONORID) REFERENCES user(ID) ON DELETE CASCADE ON UPDATE CASCADE"
            };

            createTable("item", itemTable, itemForeignKeys);

            vector<Column> requestTable = {
                {"ID", "VARCHAR", 10, "PRIMARY KEY"},
                {"RECIPIENTID", "INT", 10, "NOT NULL"},
                {"ITEMID", "INT", 10, "NOT NULL"},
                {"AMOUNT", "INT", 15, "NOT NULL"},
                {"REQUESTADDRESS", "TEXT", 0, "NOT NULL"},
                {"REQUESTDATE", "DATETIME", 0, "NOT NULL"},
                {"DESCRIPTION", "TEXT", 0, "DEFAULT NULL"},
                {"STATUS", "ENUM('pending', 'failed', 'approved')", 0, "NOT NULL DEFAULT 'pending'"},
            };
            vector<string> requestForeignKeys = {
                "FOREIGN KEY (RECIPIENTID) REFERENCES user(ID) ON DELETE CASCADE ON UPDATE CASCADE",
                "FOREIGN KEY (ITEMID) REFERENCES item(ID) ON DELETE CASCADE ON UPDATE CASCADE"
            };

            createTable("request", requestTable, requestForeignKeys);

            PopulateDatabase();
        }

        static void createTable(const string& tableName, const vector<Column>& columns, const vector<string>& foreignKeys = {}) {
            string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
            for (size_t i = 0; i < columns.size(); ++i) {
                query += columns[i].name + " " + columns[i].type;
                if (columns[i].size > 0)
                    query += "(" + to_string(columns[i].size) + ")";
                if (!columns[i].extras.empty())
                    query += " " + columns[i].extras;
                query += ", ";
            }

            for (size_t i = 0; i < foreignKeys.size(); ++i) {// Add foreign key constraints, if any
                query += foreignKeys[i];
                if (i != foreignKeys.size() - 1)
                    query += ", ";
            }
            // Remove trailing comma if present
            if (query.back() == ' ' && query[query.size()-2] == ',')
                query = query.substr(0, query.size()-2);
            else if (query.back() == ',')
                query.pop_back();
            query += ");";

            qstate = mysql_query(conn, query.c_str());
            if (qstate != 0) {
                cout << "Error creating table " << tableName << ": " << mysql_error(conn) << endl;
            }
        }
};