#pragma once
#include "Variable Token.h"
#include "Function Token.h"
class SymbolTable
{
    static vector<VariableToken> variableTable;
    static vector<FunctionToken> functionTable;

public:
    static void pushVariable(string name, int scope)
    {
        variableTable.push_back(VariableToken(variableTable.size(), name, scope));
    }

    static vector<VariableToken> getVariableTable()
    {
        return variableTable;
    }

    static void pushFunction(string name)
    {
        functionTable.push_back(FunctionToken(functionTable.size(), name));
    }

    static vector<FunctionToken> getFunctionTable()
    {
        return functionTable;
    }

    static void displayVariableTable()
    {
        cout << "Variable Table" << endl;
        for (VariableToken token : variableTable)
            token.display();
        cout << endl;
    }

    static void displayFunctionTable()
    {
        cout << "Function Table" << endl;
        for (FunctionToken token : functionTable)
            token.display();
        cout << endl;
    }
};
vector<VariableToken> SymbolTable::variableTable;
vector<FunctionToken> SymbolTable::functionTable;