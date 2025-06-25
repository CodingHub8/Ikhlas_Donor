#include "Utilities.h"
#include "DBConnection.h"
#include "User.h"
#include "Admin.h"
#include "Item.h"
#include "Request.h"
using namespace std;

// Reusable functions start
void printBar(const string& label, double amount, double scale) {
	cout << setw(17) << left << label << " | ";
	int count = static_cast<int>(round(amount / scale));
	for (int i = 0; i < count; ++i) cout << "#";
	cout << " (" << amount << ")" << endl;
}
// Reusable functions end

// Item section start
void itemManagement(User& user) {
	system("cls");//clear text
	int choice;
	int viewChoice;
	Item item;

	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------ ITEM ------------------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Add Item                            +" << endl;
	cout << "+ 2. Edit Item                           +" << endl;
	cout << "+ 3. View Item                           +" << endl;
	cout << "+ 4. Delete Item                         +" << endl;
	cout << "+ 0. Back                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	string query = "SELECT * FROM item WHERE DONORID = '" + string(user.getID()) + "'";
	switch(choice){
		case 1:// Add item
			if (item.addItem(user)) {
				cout << "Item added successfully!" << endl;
			} else {
				cout << "Failed to add item. Please try again." << endl;
			}
			break;
		case 2:// Edit item
			item.viewAllItems(query, "");//view all items to get ID
			item.viewItem();//view item to get name and price
			if (item.editItem(user)) {
				cout << "Item updated successfully." << endl;
			} else {
				cout << "Failed to update item. Please try again." << endl;
			}
			break;
		case 3:// View item
			do {
				system("cls");//clear text
				cout << "1. Search for an item" << endl;
				cout << "2. View all donated items" << endl;
				cout << "0. Back" << endl;
				cout << "Please choose from the option(s) above: ";
				inputint(viewChoice);

				if (viewChoice == 1) {
					item.viewItem();
					system("pause");//pause to view the result
				} else if (viewChoice == 2) {
					item.viewAllItems(query, "");
					system("pause");//pause to view the result
				} else if (viewChoice == 0) {
					break;
				} else {
					cout << "Invalid choice. Please try again." << endl << endl;
					system("pause");//pause to view the result
				}
			} while (true);
			break;
		case 4:// Delete item
			item.viewAllItems(query, "");//view all items to get ID
			if (item.deleteItem(user)) {
				cout << "Item deleted successfully." << endl;
			} else {
				cout << "Item deletion failed or cancelled." << endl;
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	system("pause");//pause to view the result
	itemManagement(user);
}
// Item section ends

// User section start
void userOptions(User&);
void donorOptions(User&);
void recipientOptions(User&);

void userMenu(){
	system("cls");//clear text

	int choice;
	User user;

	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------ USER ------------------" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Login                               +" << endl;
	cout << "+ 2. Sign Up                             +" << endl;
	cout << "+ 0. Back                                +" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";

	inputint(choice);

	switch(choice){
		case 1:
			cout << "+++++ USER LOGIN +++++" << endl << endl;
			if(user.login()){
				userOptions(user);
			} else {
				cout << "Invalid login credentials" << endl;
				system("pause");
			}
			break;
		case 2:
			cout << "+++++ USER SIGN UP +++++" << endl;
			user.signup();
			system("pause");
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}

	userMenu();
}

void userOptions(User& user) {
	system("cls");//clear text

	cout << "WELCOME, " << user.getName() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	if (toLowerCase(user.getRole()) == "donor") {
		donorOptions(user);
	} else if (toLowerCase(user.getRole()) == "recipient") {
		recipientOptions(user);
	}
}

// Donor
void donationReport(User&);

void donorOptions(User& user) {
	int choice;
	cout << "------------------   DONOR   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Item Management                          +" << endl;
	cout << "+ 2. View Donation Report                     +" << endl;
	cout << "+ 3. Edit Profile                             +" << endl;
	cout << "+ 4. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1:
			itemManagement(user);
			break;
		case 2:
			donationReport(user);
			break;
		case 3:
			user.editProfile();
			break;
		case 4:
			if (user.deleteProfile()) {
				userMenu();
			}
			break;
		case 0: return;//return to menu page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}
	system("pause");//pause to view the result
	userOptions(user);
}

void donationReport(User& user) {//TODO: Fix some calculations
    system("cls"); //clear text
	cout << "-----   DONOR   -----" << endl;
    cout << "1. Monthly" << endl;
    cout << "2. Quarterly" << endl;
    cout << "3. Yearly" << endl;
    cout << "0. Back" << endl;
    cout << "Please choose from the option(s) above: ";
    int choice;
    inputint(choice);

    if (choice == 0) return;

    int year, month = 0, quarter = 0;
    string periodTitle, periodStr;
    switch (choice) {
        case 1:
            cout << "Monthly report" << endl;
            cout << "Enter year: ";
            inputint(year);
            cout << "Enter month (1-12): ";
            inputint(month);
            periodTitle = "Monthly Report for " + to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month);
            periodStr = to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month);
            break;
        case 2:
            cout << "Quarterly report" << endl;
            cout << "Enter year: ";
            inputint(year);
            cout << "Enter quarter (1-4): ";
            inputint(quarter);
            periodTitle = "Quarterly Report for Q" + to_string(quarter) + " " + to_string(year);
            periodStr = "Q" + to_string(quarter) + " " + to_string(year);
            break;
        case 3:
            cout << "Yearly report" << endl;
            cout << "Enter year: ";
            inputint(year);
            periodTitle = "Yearly Report for " + to_string(year);
            periodStr = to_string(year);
            break;
        default:
            cout << "Invalid choice. Please try again." << endl << endl;
            donationReport(user); // recursion
            return;
    }

    vector<string> categories = {"Food", "Clothing", "Toy", "Money"}; // Common categories

    // Build date condition based on period
    string dateCondition;
    if (choice == 1) { // Monthly
        dateCondition = "YEAR(item.DATEADDED) = " + to_string(year) +
                       " AND MONTH(item.DATEADDED) = " + to_string(month);
    } else if (choice == 2) { // Quarterly
        int startMonth = (quarter - 1) * 3 + 1;
        dateCondition = "YEAR(item.DATEADDED) = " + to_string(year) +
                       " AND MONTH(item.DATEADDED) BETWEEN " + to_string(startMonth) +
                       " AND " + to_string(startMonth + 2);
    } else { // Yearly
        dateCondition = "YEAR(item.DATEADDED) = " + to_string(year);
    }

    string query = "SELECT item.ID, item.NAME, item.AMOUNT, item.CATEGORY, item.DATEADDED, "
                   "request.ID, request.AMOUNT, request.REQUESTDATE, request.STATUS "
                   "FROM item INNER JOIN request ON item.ID = request.ITEMID "
                   "WHERE item.DONORID = '" + string(user.getID()) + "' "
                   "AND (request.STATUS='approved' OR request.STATUS ='pending') "
                   "AND " + dateCondition;

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching data: " << mysql_error(conn) << endl;
        return;
    }
    res = mysql_store_result(conn);

    Item item; vector<Item> items;
    Request request;

    // Initialize data structures to store amounts
    map<string, int> donatedAmounts;
    map<string, int> requestedAmounts;
    map<string, string> mostRequestedItems; // category -> item name
    map<string, int> itemRequestCounts;     // item name -> count

    for (const auto& cat : categories) {
        donatedAmounts[cat] = 0;
        requestedAmounts[cat] = 0;
    }

    while ((row = mysql_fetch_row(res))) {
        item.setID(row[0]);            item.setDonorID(user.getID()); item.setName(row[1]);
        item.setAmount(atoi(row[2]));  item.setCategory(row[3]);      item.setDateAdded(row[4]);
    	items.push_back(item);

        request.setID(row[5]);			   request.setItemID(item.getID()); request.setAmount(atoi(row[6]));
    	request.setRequestDate(row[7]); request.setStatus(row[8]);

        // Update donated amounts
        donatedAmounts[item.getCategory()] += item.getAmount();

        // Update requested amounts
        requestedAmounts[item.getCategory()] += request.getAmount();

        // Track most requested items
        itemRequestCounts[item.getName()]++;
        if (!mostRequestedItems.contains(item.getCategory()) ||
            itemRequestCounts[item.getName()] > itemRequestCounts[mostRequestedItems[item.getCategory()]]) {
            mostRequestedItems[item.getCategory()] = item.getName();
        }
    }
    mysql_free_result(res);

    // Find max value for scaling the graph
    int maxAmount = 0;
    for (const auto& cat : categories) {
        if (donatedAmounts[cat] > maxAmount) maxAmount = donatedAmounts[cat];
        if (requestedAmounts[cat] > maxAmount) maxAmount = requestedAmounts[cat];
    }

    // Calculate scale factor (each # represents this many units)
    int scale = max(1, maxAmount / 30); // Aim for about 30 characters max width

    // Print the report header
    cout << "\n" << periodTitle << "\n";
    cout << string(periodTitle.length(), '=') << "\n\n";

    // Print the horizontal bar graph
    cout << "Donation vs Request Comparison (Scale: 1 '#' = " << scale << " units)\n";
    cout << "--------------------------------------------------\n";

    for (const auto& cat : categories) {
        // Donated bar
        cout << "Donated " << left << setw(10) << cat << " | ";
        int donatedBars = donatedAmounts[cat] / scale;
        cout << string(donatedBars, '#') << " (" << donatedAmounts[cat] << ")\n";

        // Requested bar
        cout << "Requested " << left << setw(8) << cat << " | ";
        int requestedBars = requestedAmounts[cat] / scale;
        cout << string(requestedBars, '#') << " (" << requestedAmounts[cat] << ")\n";

        cout << "\n";
    }

    // Additional analysis
    cout << "\nAnalysis:\n";
    cout << "---------\n";

    // Find most requested category
    string maxRequestedCat;
    int maxRequests = 0;
    for (const auto& cat : categories) {
        if (requestedAmounts[cat] > maxRequests) {
            maxRequests = requestedAmounts[cat];
            maxRequestedCat = cat;
        }
    }

    // Find most donated category
    string maxDonatedCat;
    int maxDonated = 0;
    for (const auto& cat : categories) {
        if (donatedAmounts[cat] > maxDonated) {
            maxDonated = donatedAmounts[cat];
            maxDonatedCat = cat;
        }
    }

    if (!items.empty()) {
        cout << "- Most requested item category in " << periodStr << " is " << maxRequestedCat
             << " with " << maxRequests << " items requested.\n";

        cout << "- Highest amount of successfully donated item category is " << maxDonatedCat
             << " with " << maxDonated << " items donated.\n";

        // Print most requested item in each category
        for (const auto& cat : categories) {
            if (mostRequestedItems.contains(cat)) {
                cout << "- Most requested " << cat << " item: " << mostRequestedItems[cat]
                     << " (" << itemRequestCounts[mostRequestedItems[cat]] << " requests)\n";
            }
        }

        // Compare donated vs requested
        int totalDonated = 0, totalRequested = 0;
        for (const auto& cat : categories) {
            totalDonated += donatedAmounts[cat];
            totalRequested += requestedAmounts[cat];
        }

        if (totalDonated > totalRequested) {
            cout << "- More items were donated (" << totalDonated << ") than requested ("
                 << totalRequested << ") during this period.\n";
        } else if (totalDonated < totalRequested) {
            cout << "- More items were requested (" << totalRequested << ") than donated ("
                 << totalDonated << ") during this period.\n";
        } else {
            cout << "- Donation and request amounts are equal (" << totalDonated
                 << ") for this period.\n";
        }
    } else {
        cout << "No donation data found for the selected period.\n";
    }

    system("pause");
    donationReport(user); // Return to the menu after pause
}

// Recipient
void recipientOptions(User& user) {
	int choice;
	Item item;
	Request request;

	cout << "------------------ RECIPIENT ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Request Donation                         +" << endl;
	cout << "+ 2. View Request Status                      +" << endl;
	cout << "+ 3. Cancel Request                           +" << endl;
	cout << "+ 4. Edit Profile                             +" << endl;
	cout << "+ 5. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1://Request donation
			item.viewAllItems("SELECT * FROM item", "");//view all items
			if (request.createRequest(user)) {
				cout << "Request created!" << endl;
			} else {
				cout << "Request failed or cancelled. Please try again." << endl;
			}
			break;
		case 2://Request status
			request.viewAllRequests("SELECT * FROM request WHERE RECIPIENTID = '" + string(user.getID()) + "'");
			break;
		case 3://Cancel request
			request.viewAllRequests("SELECT * FROM request WHERE RECIPIENTID = '" + string(user.getID()) + "' AND STATUS = 'pending'");
			if (request.cancelRequest(user)) {
				cout << "Request cancelled!" << endl;
			} else {
				cout << "Cancellation failed!" << endl;
			}
			break;
		case 4://Edit profile
			user.editProfile(); break;
		case 5://Delete profile
			if (user.deleteProfile()) {
				userMenu();
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}
	system("pause");//pause to view the result
	userOptions(user);
}
// User section ends

// Admin section start
void adminOptions(Admin&);
void processRecipientRequest();
void viewOverallReport();

void adminMenu() {
	system("cls");//clear text

	int choice;
	Admin admin;

	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "----------------- ADMIN -----------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Login                              +" << endl;
	cout << "+ 2. Sign Up                            +" << endl;
	cout << "+ 0. Back                               +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";

	inputint(choice);

	switch(choice){
		case 1:
			cout << "+++++ ADMIN LOGIN +++++" << endl << endl;
			if(admin.login()){
				adminOptions(admin);
			} else {
				cout << "Invalid login credentials" << endl;
			}
			break;
		case 2:
			cout << "+++++ ADMIN SIGN UP +++++" << endl;
			admin.signup();
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl;
	}
	system("pause");
	adminMenu();
}

void adminOptions(Admin& admin) {
	system("cls");//clear text
	int choice;
	int viewChoice;
	User user;
	Item item;
	Request request;

	cout << "WELCOME, " << admin.getUsername() << "!" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "------------------   ADMIN   ------------------" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+ 1. Approve/Reject Pending Request           +" << endl;
	cout << "+ 2. View Report                              +" << endl;
	cout << "+ 3. View User Profile                        +" << endl;
	cout << "+ 4. View All Requests                        +" << endl;
	cout << "+ 5. View All Items                           +" << endl;
	cout << "+ 6. Edit Profile                             +" << endl;
	cout << "+ 7. Delete Profile                           +" << endl;
	cout << "+ 0. Back                                     +" << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	cout << "Please choose from the option(s) above: ";
	inputint(choice);

	system("cls");//clear text
	switch(choice){
		case 1://Approve pending requests
			request.viewAllRequests("SELECT * FROM request WHERE STATUS = 'pending'");//Only view pending requests
			processRecipientRequest();
			break;
		case 2://View report
			viewOverallReport();
			break;
		case 3://View user profile
			user.viewProfile();
			break;
		case 4://View Requests
			do {
				system("cls");//clear text
				cout << "1. Search for a request" << endl;
				cout << "2. View all requests" << endl;
				cout << "0. Back" << endl;
				cout << "Please choose from the option(s) above: ";
				inputint(viewChoice);

				if (viewChoice == 1) {
					request.viewRequest();
					system("pause");//pause to view the result
				} else if (viewChoice == 2) {
					request.viewAllRequests("SELECT * FROM request");
					system("pause");//pause to view the result
				} else if (viewChoice == 0) {
					break;
				} else {
					cout << "Invalid choice. Please try again." << endl << endl;
					system("pause");//pause to view the result
				}
			} while (true);
			break;
		case 5://View Items
			do {
				system("cls");//clear text
				cout << "1. Search for an item" << endl;
				cout << "2. View all donated items" << endl;
				cout << "0. Back" << endl;
				cout << "Please choose from the option(s) above: ";
				inputint(viewChoice);

				if (viewChoice == 1) {
					item.viewItem();
					system("pause");//pause to view the result
				} else if (viewChoice == 2) {
					item.viewAllItems("SELECT * FROM item", "Admin");
					system("pause");//pause to view the result
				} else if (viewChoice == 0) {
					break;
				} else {
					cout << "Invalid choice. Please try again." << endl << endl;
					system("pause");//pause to view the result
				}
			} while (true);
			break;
		case 6:
			admin.editProfile();
			break;
		case 7:
			if (admin.deleteProfile()) {
				system("pause");//pause to view the result
				return;
			}
			break;
		case 0: return;//return to previous page
		default: cout << "Invalid choice. Please try again." << endl << endl; break;
	}

	system("pause");//pause to view the result
	adminOptions(admin);
}

void processRecipientRequest() {
	char requestID[10];
	string status;
	string query = "SELECT ID FROM request WHERE STATUS = 'pending'";

	do {
		cout << "Please enter the request ID to process (Enter '0' to cancel): ";
		cin >> requestID;
		if (strcmp(requestID, "0") == 0) return;

		if (mysql_query(conn, query.c_str()) != 0) {
			cout << "Error fetching request ID: " << mysql_error(conn) << endl;
			return;
		}
		res = mysql_store_result(conn);
		bool found = false;
		if (res) {
			while ((row = mysql_fetch_row(res))) {
				if (strcmp(row[0], requestID) == 0) {
					found = true;
					break;
				}
			}
			mysql_free_result(res);
		}
		if (!found) {
			cout << "Request ID not found. Please enter a valid request ID" << endl;
		} else {
			break;
		}
	}while (true);

	query = "SELECT u.* FROM request r JOIN user u ON r.RECIPIENTID = u.ID WHERE r.ID = '" + string(requestID) + "'";
	if (mysql_query(conn, query.c_str()) != 0) {
		cout << "Error fetching user details: " << mysql_error(conn) << endl;
		return;
	}
	res = mysql_store_result(conn);
	if (res) {
		row = mysql_fetch_row(res);
		if (row) {//will display recipient profile to ease admin's task in making decisions before approving/rejecting
			cout << "---------- Recipient Profile ----------" << endl;
			cout << "ID         : " << row[0] << endl;
			cout << "Name       : " << row[1] << endl;
			cout << "Email      : " << row[2] << endl;
			cout << "Phone      : " << (row[4] ? row[4] : "NULL") << endl;
			cout << "Address    : " << (row[5] ? row[5] : "NULL") << endl;
			cout << "Role       : " << row[6] << endl;
			cout << "---------------------------------------" << endl;
		} else {
			cout << "No user found for this request." << endl;
		}
		mysql_free_result(res);
	} else {
		cout << "No user found for this request." << endl;
	}

	strcpy(requestID, toUpperCase(requestID).c_str());

	int choice;
	cout << "Do you want to [1]Approve or [2]Reject this request? (Enter '0' to cancel)" << endl;
	cout << "-> ";
	inputint(choice);

	if (choice == 1) {
		status = "approved";
	} else if (choice == 2) {
		status = "failed";
	} else if (choice == 0) {
		return;
	}

	query = "SELECT r.AMOUNT as reqAmount, i.AMOUNT as itemAmount, r.ITEMID FROM request r JOIN item i ON r.ITEMID = i.ID WHERE r.ID = '"
			+ string(requestID) + "'";
	if (mysql_query(conn, query.c_str()) != 0) {
		cout << "Error fetching amounts: " << mysql_error(conn) << endl;
		return;
	}
	res = mysql_store_result(conn);
	if (res) {
		row = mysql_fetch_row(res);
		int reqAmount = atoi(row[0]);
		int itemAmount = atoi(row[1]);
		string itemID = row[2];
		int finalAmount = itemAmount - reqAmount;
		mysql_free_result(res);

		// Update request status
		query = "UPDATE request SET STATUS = '" + status + "' WHERE ID = '" + string(requestID) + "'";
		if (mysql_query(conn, query.c_str()) != 0) {
			cout << "Error updating request: " << mysql_error(conn) << endl;
			return;
		}

		// Update item amount
		query = "UPDATE item SET AMOUNT = " + to_string(finalAmount) + " WHERE ID = '" + itemID + "'";
		if (mysql_query(conn, query.c_str()) != 0) {
			cout << "Error updating item amount: " << mysql_error(conn) << endl;
			return;
		}
	}

	cout << "Request processed successfully!" << endl;
}

void viewOverallReport() {//TODO: Fix calculations
    int choice;
    int year, month = 0, quarter = 0;
    string periodTitle, filename;
    string dateCondition;
	int startMonth;

	do {
		system("cls");//clear text
		cout << "-----   ADMIN   -----" << endl;
		cout << "1. Monthly Report" << endl;
	    cout << "2. Quarterly Report" << endl;
	    cout << "3. Yearly Report" << endl;
	    cout << "0. Back" << endl;
	    cout << "Please choose the report to view: ";
	    inputint(choice);

	    if (choice == 0) return;
		if (choice < 1 || choice > 3) {
			cout << "Invalid choice. Please try again." << endl << endl;
			system("pause");//pause to view the result
			continue;
		}

	    cout << "Enter year: ";
	    inputint(year);

		if (choice == 1) {
			// Monthly
			cout << "Enter month (1-12): ";
			inputint(month);
			periodTitle = "Monthly Report for " + to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month);
			filename = toLowerCase(getMonthName(month)) + "_" + to_string(year) + "_report.csv";
			dateCondition = "YEAR(item.DATEADDED) = " + to_string(year) +
			                " AND MONTH(item.DATEADDED) = " + to_string(month);
			break;
		} if (choice == 2) {
			// Quarterly
			cout << "Enter quarter (1-4): ";
			inputint(quarter);
			periodTitle = "Quarterly Report for Q" + to_string(quarter) + " " + to_string(year);
			filename = "quarter" + to_string(quarter) + "_" + to_string(year) + "_report.csv";
			startMonth = (quarter - 1) * 3 + 1;
			dateCondition = "YEAR(item.DATEADDED) = " + to_string(year) +
			                " AND MONTH(item.DATEADDED) BETWEEN " + to_string(startMonth) +
			                " AND " + to_string(startMonth + 2);
			break;
		} if (choice == 3) {
			// Yearly
			periodTitle = "Yearly Report for " + to_string(year);
			filename = to_string(year) + "_report.csv";
			dateCondition = "YEAR(item.DATEADDED) = " + to_string(year);
			break;
		}
	}while (true);
	
    // Query to get all donations and requests for the period
    string query = "SELECT "
                   "item.CATEGORY, "
                   "COUNT(item.ID) as donation_count, "
                   "SUM(item.AMOUNT) as total_donated, "
                   "COUNT(CASE WHEN request.STATUS = 'approved' THEN 1 ELSE NULL END) as approved_count, "
                   "COUNT(CASE WHEN request.STATUS = 'pending' THEN 1 ELSE NULL END) as pending_count, "
                   "COUNT(CASE WHEN request.STATUS = 'failed' THEN 1 ELSE NULL END) as failed_count "
                   "FROM item JOIN request ON item.ID = request.ITEMID "
                   "WHERE " + dateCondition + " "
                   "GROUP BY item.CATEGORY";

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching report data: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "No data found for the selected period." << endl;
    } else {
    	// Create and open CSV file
		filename = "Reports/" + filename;
	    if (filesystem::path pathObj(filename); pathObj.has_parent_path()) {
			filesystem::create_directories(pathObj.parent_path());
		}
	    ofstream csvFile(filename, ios::out);
	    if (!csvFile.is_open()) {
	        cout << "Error creating CSV file." << endl;
	        mysql_free_result(res);
	        return;
	    }

	    // Write CSV header
	    csvFile << "Category,Donations Made,Total Units Donated,Approved Requests,Pending Requests,Failed Requests" << endl;

	    // Print report header to console
	    cout << "\n" << periodTitle << "\n";
	    cout << string(periodTitle.length(), '=') << "\n\n";
	    cout << left << setw(12) << "Category"
	         << right << setw(15) << "Donations Made"
	         << setw(22) << "Total Units Donated"
	         << setw(17) << "Approved Requests"
	         << setw(17) << "Pending Requests"
	         << setw(17) << "Failed Requests" << endl;
	    cout << setw(100) << setfill('-') << "" << setfill(' ') << endl;

	    // Process each row of data
		string category;
	    double totalDonated = 0, donationCount = 0;
    	double totalRequested = 0, totalApproved = 0, totalPending = 0, totalFailed = 0;
	    while ((row = mysql_fetch_row(res))) {
		    category = row[0] ? row[0] : "Unknown";
		    donationCount = row[1] ? atof(row[1]) : 0;
		    totalDonated = row[2] ? atof(row[2]) : 0;
		    totalApproved = row[3] ? atof(row[3]) : 0;
		    totalPending = row[4] ? atof(row[4]) : 0;
		    totalFailed = row[5] ? atof(row[5]) : 0;
		    totalRequested += totalApproved + totalPending + totalFailed;

		    // Print to console
		    cout << left << setw(12) << category
				    << right << setw(15) << donationCount
				    << setw(22) << totalDonated
				    << setw(17) << totalApproved
				    << setw(17) << totalPending
				    << setw(17) << totalFailed << endl;

		    // Write to CSV
		    csvFile << category << ","
				    << donationCount << ","
				    << totalDonated << ","
				    << totalApproved << ","
				    << totalPending << ","
				    << totalFailed << endl;
	    }

	    // Close CSV file
	    csvFile.close();
	    mysql_free_result(res);

	    // Analysis section (add this after values have been calculated)
	    if (totalDonated > totalRequested) {
		    cout << "- More items were donated (" << totalDonated << ") than requested (" << totalRequested << ") during this period.\n";
	    } else if (totalDonated < totalRequested) {
		    cout << "- More items were requested (" << totalRequested << ") than donated (" << totalDonated << ") during this period.\n";
	    } else {
		    cout << "- Donation and request amounts are equal (" << totalDonated << ") for this period.\n";
	    }
	    // Print summary
	    cout << "\nReport saved as: " << filename << endl;
	    cout << "Location: " << filesystem::current_path().string() << "\\" << filename << endl;
    }

    system("pause");
	viewOverallReport(); // Return to the menu after pause
}
// Admin section ends

int main() {
	DBConnection::InitializeDatabase();
	system("pause");

	int choice;

	do{
		system("cls");//clear text, data still remains
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "----------- IKHLAS DONOR SYSTEM ----------" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+ 1. User                                +" << endl;
		cout << "+ 2. Admin                               +" << endl;
		cout << "+ 0. Exit                                +" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
		cout << "Please choose from the option(s) above: ";
		inputint(choice);

		switch(choice){
			case 1://user
				userMenu();
				break;
			case 2://admin
				adminMenu();
				break;
			case 0:
				cout << "THANK YOU FOR USING IKHLAS DONOR! :)" << endl;
				mysql_close(conn);
				system("pause");
				exit(0);
			default: cout << "Invalid choice. Please try again." << endl << endl; system("pause"); break;
		}
	}while(true);
}