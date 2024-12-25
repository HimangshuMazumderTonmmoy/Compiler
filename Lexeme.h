#pragma once

#include <iostream>
using namespace std;

class Lexeme
{
public:
    string name;
    string type;

    Lexeme(string name, string type) : name(name), type(type) {}
    void display()
    {
        cout << "[" << name << " : " << type << "] ";
    }
};

ostream &operator<<(ostream &os, Lexeme lexeme)
{
    cout << "[" << lexeme.name << " : " << lexeme.type << "] ";
    return os;
}