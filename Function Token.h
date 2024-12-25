#pragma once

#include <iostream>
#include <vector>
#include "Token.h"
using namespace std;

class FunctionToken : public Token
{
public:
    string returnType;
    vector<string> typeOfParameters;
    vector<string> methodOfParameterPassing;

    FunctionToken(int location, string name) : Token(location, name) {}

    string getToken() override
    {
        return ("<Function," + to_string(location) + ">");
    }

    string parameters()
    {
        if (typeOfParameters.size() == 0)
            return " ()";

        string parameters = " (";
        for (int i = 0; i < typeOfParameters.size(); i++)
            parameters += "[" + typeOfParameters[i] + " : " + methodOfParameterPassing[i] + "], ";
        parameters[parameters.size() - 2] = ')';
        parameters.erase(parameters.begin() + (parameters.size() - 1));
        return parameters;
    }

    void display() override
    {
        cout << location
             << " - "
             << returnType << " "
             << name
             << parameters()
             << endl;
    }
};