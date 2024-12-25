#pragma once

#include <iostream>
using namespace std;

class Token
{
public:
    int location;
    string name;

    Token() {}
    Token(int location, string name) : location(location), name(name) {}
    virtual string getToken() = 0;
    virtual void display() = 0;
};