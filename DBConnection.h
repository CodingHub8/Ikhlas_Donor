#include <mysql.h>
#include <mysqld_error.h>
using namespace std;

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
                {"DONORID", "INT", 10, "NOT NULL"},
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