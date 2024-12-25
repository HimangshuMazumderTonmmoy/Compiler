#pragma once

#include <iostream>
using namespace std;

class Error
{
public:
    int line;
    string segment;

    string type;
    int serialNo;

    Error(int line, string type, int serialNo, string segment = "") : line(line), type(type), serialNo(serialNo), segment(segment) {}
};