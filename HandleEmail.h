#pragma once
#include <string>

class HandleEmail
{
public:
    static int HandleSignup(const std::string& firstName, const std::string& password, const std::string& pin, const std::string& email);
    static int HandleForgotPassword(const std::string& firstName, const std::string& password, const std::string& email);

private:
    static int sendEmail(const std::string& firstName, const std::string& subject, const std::string& content, const std::string& email);
};
