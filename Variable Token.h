#pragma once

#include <iostream>
#include "Token.h"
using namespace std;

class VariableToken : public Token
{
public:
    int size, scope;
    string type;

    VariableToken(int location, string name, int scope) : Token(location, name), scope(scope) {}

    string getToken() override
    {
        return ("<ID," + to_string(location) + ">");
    }

    void display() override
    {
        cout << location
             << " - "
             << type
             << "(" << size << ") "
             << name
             << " {Function " << scope << "}"
             << endl;
    }
};