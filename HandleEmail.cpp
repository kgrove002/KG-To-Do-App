#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include "HandleEmail.h"

using json = nlohmann::json;

int HandleEmail::sendEmail(const std::string& firstName, const std::string& subject, const std::string& content, const std::string& email) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;

    // Construct the JSON data for the SendGrid API
    json jsonData;
    jsonData["personalizations"] = { {{"to", {{{"email", email}}}}} };
    jsonData["from"] = { {"email", "bqcritken@gmail.com"} };
    jsonData["subject"] = subject;
    jsonData["content"] = { {{"type", "text/plain"}, {"value", content}} };

    std::string jsonString = jsonData.dump();

    //SendGrid API key
    std::string apiKey = "API KEY";

    // Initialize CURL
    curl = curl_easy_init();
    if (curl) {
        // Set the URL for the SendGrid API
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.sendgrid.com/v3/mail/send");

        // Set the headers for the HTTP request
        std::string authHeader = "Authorization: Bearer ";
        authHeader += apiKey;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the data to be sent in the HTTP POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    else {
        std::cerr << "Failed to initialize curl" << std::endl;
        return -1; // Return a distinct error code if CURL initialization fails
    }

    return (int)res;
}

int HandleEmail::HandleSignup(const std::string& firstName, const std::string& password, const std::string& pin, const std::string& email) {
    // Construct the email content
    std::stringstream emailContent;
    emailContent << "Thank you, " << firstName << " for signing up with our to do app. Below is your information in case you wish to save it at a later time. "
        << "Please keep your pin handy as it is the only way to recover your account.\n"
        << "Email: " << email << "\n"
        << "Password: " << password << "\n"
        << "Pin: " << pin << "\n"
        << "Please enjoy the use of this application. For any questions or requests, please email bqcritken@gmail.com to contact the developer of this free app.";

    // Send the email using sendEmail method
    return sendEmail(firstName, "Welcome to KG To Do App!", emailContent.str(), email);
}

int HandleEmail::HandleForgotPassword(const std::string& firstName, const std::string& password, const std::string& email) {
    // Construct the email content
    std::stringstream emailContent;
    emailContent << "Hello, " << firstName << ",\n We are sorry you have lost your password. Your password is\nPassword: " << password
        << "\n Please keep your password handy or need be update it in the app. Thank you again for your continue support.\n";

    // Send the email using sendEmail method
    return sendEmail(firstName, "Forgot Password to the KG To Do App!", emailContent.str(), email);
}
