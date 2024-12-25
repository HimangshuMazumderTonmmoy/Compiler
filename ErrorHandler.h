#pragma once

#include <iostream>
#include <vector>
#include "Error.h"
using namespace std;

class ErrorHandler
{
    static vector<Error> errorList;

public:
    static void pushError(int line, string type, int serialNo, string segment)
    {
        errorList.push_back(Error(line, type, serialNo, segment));
    }

    static int NumberOfError()
    {
        return errorList.size();
    }

    static void report()
    {
        for (Error error : errorList)
        {
            cout << error.type << " Error Occured in Line " << error.line << endl;
            if (error.type == "Lexical")
            {
                cout << error.segment << " is a Identifier and a Identifier can't have any ";
                if (error.serialNo == 1)
                    cout << "number at the beginning";
                else if (error.serialNo == 2)
                    cout << "special symbol";
                cout << endl
                     << endl;
            }
        }

        if (NumberOfError() > 0)
            exit(0);
    }
};

vector<Error> ErrorHandler::errorList;