#include "DatabaseConnect.h"
#include <pqxx/pqxx>
#include <iostream>

// Static member initialization
int DatabaseConnect::id = 0;
std::string DatabaseConnect::firstName = "";
std::string DatabaseConnect::lastName = "";
std::string DatabaseConnect::pin = "";
std::string DatabaseConnect::password = "";
std::string DatabaseConnect::email = "";

// Define the PostgreSQL connection string
const std::string connectionString = "A Connection String";

// Establishes connection to the database
DatabaseConnect::Status DatabaseConnect::connectToDataBase() {
    try {
        pqxx::connection conn(connectionString);

        if (conn.is_open()) {
            return SUCCESS;
        }
        else {
            return FAILURE;
        }
    }
    catch (const std::exception&) {
        return EXCEPTION;
    }
}

// Sets up user information based on email and password
void DatabaseConnect::setupUserInfo(const std::string& emailInput, const std::string& passwordInput) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL SELECT query to retrieve user information
        pqxx::result result = txn.exec(
            "SELECT \"Customer_ID\", \"First_Name\", \"PIN\", \"Password\", \"Customer_Email\" FROM " + fullTableName +
            " WHERE \"Customer_Email\" = " + txn.quote(emailInput) + " AND \"Password\" = " + txn.quote(passwordInput)
        );

        // If a valid result is returned, update member variables
        if (!result.empty()) {
            setId(result[0][0].as<int>());
            setFirstName(result[0][1].as<std::string>());
            setPin(result[0][2].as<std::string>());
            setPassword(result[0][3].as<std::string>());
            setEmail(result[0][4].as<std::string>());
        }
    }
    catch (const std::exception&) {
        return;
    }
}

// Loads items from specified table for the logged-in user
std::pair<std::vector<std::string>, std::vector<bool>> DatabaseConnect::LoadItemsFromdb(const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL SELECT query to retrieve tasks and their status
        pqxx::result result = txn.exec(
            "SELECT \"Task_DESC\", \"Is_Checked\" FROM " + fullTableName + " WHERE \"Customer_ID\" = " + txn.quote(id)
        );

        std::vector<std::string> tasks;
        std::vector<bool> isChecked;

        // Extracts data from the result set
        for (const auto& row : result) {
            tasks.push_back(row[0].as<std::string>());
            isChecked.push_back(row[1].as<bool>());
        }

        txn.commit();

        return { tasks, isChecked };
    }
    catch (const std::exception&) {
        return { {}, {} };
    }
}

// Adds a new item to the specified table for the logged-in user
void DatabaseConnect::AddNewItemToDB(const std::string& value, const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL INSERT query to add a new item
        std::string query = "INSERT INTO " + fullTableName + " (\"Customer_ID\", \"Task_DESC\", \"Is_Checked\") VALUES (" + txn.quote(id) + ", " + txn.quote(value) + ", false);";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Deletes a selected item from the specified table for the logged-in user
void DatabaseConnect::DeleteSelectedItemFromDB(const std::string& value, const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL DELETE query to remove an item
        std::string query = "DELETE FROM " + fullTableName + " WHERE \"Task_DESC\" = " + txn.quote(value) + " AND \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Deletes all items from the specified table for the logged-in user
void DatabaseConnect::DeleteAllItemsFromTable(const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL DELETE query to remove all items
        std::string query = "DELETE FROM " + fullTableName + " WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Sets all items in the specified table to true (checked) for the logged-in user
void DatabaseConnect::SetAllItemsToTrue(const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL UPDATE query to set all items to true
        std::string query = "UPDATE " + fullTableName + " SET \"Is_Checked\" = TRUE WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Sets all items in the specified table to false (unchecked) for the logged-in user
void DatabaseConnect::SetAllItemsToFalse(const std::string& table) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL UPDATE query to set all items to false
        std::string query = "UPDATE " + fullTableName + " SET \"Is_Checked\" = FALSE WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Updates the isChecked status of a specific item in the specified table for the logged-in user
void DatabaseConnect::updateOnCheck(const std::string& table, const std::string& value, bool isChecked) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name(table);

        // SQL UPDATE query to update isChecked status
        std::string query = "UPDATE " + fullTableName + " SET \"Is_Checked\" = " + txn.quote(isChecked) + " WHERE \"Task_DESC\" = " + txn.quote(value) + " AND \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Updates the email address of the logged-in user
void DatabaseConnect::updateEmail(const std::string& value) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL UPDATE query to update email address
        std::string query = "UPDATE " + fullTableName + " SET \"Customer_Email\" = " + txn.quote(value) + " WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Updates the password of the logged-in user
void DatabaseConnect::updatePassword(const std::string& value) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL UPDATE query to update password
        std::string query = "UPDATE " + fullTableName + " SET \"Password\" = " + txn.quote(value) + " WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Updates the PIN of the logged-in user
void DatabaseConnect::updatePin(const std::string& value) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL UPDATE query to update PIN
        std::string query = "UPDATE " + fullTableName + " SET \"PIN\" = " + txn.quote(value) + " WHERE \"Customer_ID\" = " + txn.quote(id) + ";";
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Verifies user credentials (email and password)
bool DatabaseConnect::verifyUser(const std::string& emailInput, const std::string& passwordInput) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL SELECT query to verify user
        pqxx::result result = txn.exec(
            "SELECT \"Customer_ID\", \"First_Name\", \"PIN\", \"Password\", \"Customer_Email\" FROM " + fullTableName +
            " WHERE \"Customer_Email\" = " + txn.quote(emailInput) + " AND \"Password\" = " + txn.quote(passwordInput)
        );

        // Returns true if a valid user is found
        return !result.empty();
    }
    catch (const std::exception&) {
        return false;
    }
}

// Checks if an email already exists in the database
bool DatabaseConnect::checkForExistingEmail(const std::string& email) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL SELECT query to check for existing email
        pqxx::result result = txn.exec(
            "SELECT \"Customer_ID\", \"First_Name\", \"PIN\", \"Password\", \"Customer_Email\" FROM " + fullTableName +
            " WHERE \"Customer_Email\" = " + txn.quote(email)
        );

        // Returns true if email does not exist
        return result.empty();
    }
    catch (const std::exception&) {
        return false;
    }
}

// Creates a new user in the database
void DatabaseConnect::createNewUser(const std::string& firstName, const std::string& lastName, const std::string& pin, const std::string& password, const std::string& email) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL INSERT query to create a new user
        std::string query = "INSERT INTO " + fullTableName +
            " (\"First_Name\", \"Last_Name\", \"PIN\", \"Password\", \"Customer_Email\") VALUES (" +
            txn.quote(firstName) + ", " + txn.quote(lastName) + ", " + txn.quote(pin) + ", " +
            txn.quote(password) + ", " + txn.quote(email) + ")";

        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception&) {
        return;
    }
}

// Retrieves user password based on email and PIN
bool DatabaseConnect::getUserPassword(const std::string& email, const std::string& pin) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string fullTableName = txn.quote_name("public") + "." + txn.quote_name("Customer");

        // SQL SELECT query to retrieve user password
        pqxx::result result = txn.exec(
            "SELECT \"First_Name\", \"PIN\", \"Password\", \"Customer_Email\" FROM " + fullTableName +
            " WHERE \"Customer_Email\" = " + txn.quote(email) + " AND \"PIN\" = " + txn.quote(pin)
        );

        // If a valid result is found, update member variables and return true
        if (!result.empty()) {
            setFirstName(result[0][0].as<std::string>());
            setPin(result[0][1].as<std::string>());
            setPassword(result[0][2].as<std::string>());
            setEmail(result[0][3].as<std::string>());
            return true;
        }
        else {
            return false;
        }
    }
    catch (const std::exception&) {
        return false;
    }
}
