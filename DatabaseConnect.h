#pragma once
#include <vector>
#include <string>

class DatabaseConnect {
public:
    enum Status {
        SUCCESS,
        FAILURE,
        EXCEPTION
    };

    static Status connectToDataBase();
    static std::pair<std::vector<std::string>, std::vector<bool>> LoadItemsFromdb(const std::string& table);
    static void AddNewItemToDB(const std::string& value, const std::string& table);
    static void DeleteSelectedItemFromDB(const std::string& value, const std::string& table);
    static void DeleteAllItemsFromTable(const std::string& table);
    static void SetAllItemsToTrue(const std::string& table);
    static void SetAllItemsToFalse(const std::string& table);
    static void updateOnCheck(const std::string& table, const std::string& value, bool isChecked);
    static void updateEmail(const std::string& value);
    static void updatePassword(const std::string& value);
    static void updatePin(const std::string& value);

    static void setupUserInfo(const std::string& emailInput, const std::string& passwordInput);
    static bool verifyUser(const std::string& emailInput, const std::string& passwordInput);
    static bool checkForExistingEmail(const std::string& email);
    static void createNewUser(const std::string& firstName, const std::string& lastName, const std::string& pin, const std::string& password, const std::string& email);
    static bool getUserPassword(const std::string& email, const std::string& pin);

    // Getters
    static int getId() { return id; }
    static std::string getFirstName() { return firstName; }
    static std::string getLastName() { return lastName; }
    static std::string getPin() { return pin; }
    static std::string getPassword() { return password; }
    static std::string getEmail() { return email; }

    // Setters
    static void setId(int newId) { id = newId; }
    static void setFirstName(const std::string& newFirstName) { firstName = newFirstName; }
    static void setLastName(const std::string& newLastName) { lastName = newLastName; }
    static void setPin(const std::string& newPin) { pin = newPin; }
    static void setPassword(const std::string& newPassword) { password = newPassword; }
    static void setEmail(const std::string& newEmail) { email = newEmail; }

private:
    static int id;
    static std::string firstName;
    static std::string lastName;
    static std::string pin;
    static std::string password;
    static std::string email;
};
