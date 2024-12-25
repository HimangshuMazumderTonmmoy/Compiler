#pragma once

#include <iostream>
#include <cctype>
#include <functional>
#include "File.h"
#include "ErrorHandler.h"
#include "Lexeme.h"
#include "Token.h"
#include "Symbol Table.h"
using namespace std;

void spacing(string &line)
{
    for (int i = 0; i < line.size(); i++)
    {
        char a = line[i];
        if (((line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '=') && line[i + 1] == '=') || ((line[i] == '<' || line[i] == '>') && line[i] == line[i + 1]))
        {
            if (line[i - 1] != ' ' && i != 0)
            {
                line.insert((line.begin() + i), ' ');
                i++;
            }
            if (line[i + 2] != ' ' && i != 0)
                line.insert((line.begin() + i + 2), ' ');
            i += 2;
        }
        else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '=' || line[i] == '<' || line[i] == '>' || line[i] == '(' || line[i] == ')' || line[i] == '{' || line[i] == '}' || line[i] == ',' || line[i] == ';' || line[i] == '\'')
        {
            if (line[i - 1] != ' ' && i != 0)
            {
                line.insert((line.begin() + i), ' ');
                i++;
            }
            if (line[i + 1] != ' ' && i != 0)
                line.insert((line.begin() + i + 1), ' ');
            i++;
        }
    }
}

vector<string> splitLexeme(string line)
{
    vector<string> words;
    string word;
    for (int i = 0; i < line.size(); i++)
        if ((line[i] == ' ' || line[i] == '\n') && !word.empty())
        {
            words.push_back(word);
            word = "";
        }
        else if (line[i] == '"')
        {
            word += '"';
            for (i++; i < line.size(); i++)
            {
                if (line[i] == '"')
                    break;
                else
                    word += line[i];
            }
            word += '"';
            words.push_back(word);
            word = "";
        }
        else if (line[i] != ' ')
            word += line[i];

    if (!word.empty())
        words.push_back(word);

    return words;
}

bool isKeyword(string word)
{
    File file("Keywords.txt");
    for (string keyword : file.readWord())
        if (word == keyword)
            return true;

    return false;
}

bool isNumber(string s)
{
    for (char value : s)
        if (!isdigit(value) || value == '.')
            return false;
    return true;
}

string wordType(string word)
{
    if (word[0] == '\'')
        return "Character";
    else if (word[0] == '"')
        return "String";
    else if (word == "(" || word == ")" || word == "{" || word == "}" || word == "," || word == ";")
        return "Punctuation";
    else if (word[0] == '=' || word[0] == '+' || word[0] == '-' || word[0] == '*' || word[0] == '/' || word[0] == '<' || word[0] == '>')
        return "Operator";
    else if (word == "bool" || word == "char" || word == "short" || word == "int" || word == "long" || word == "float" || word == "double" || word == "string")
        return "Data Type";
    else if (isKeyword(word))
        return "Keyword";
    else if (isNumber(word) || word == "true" || word == "false")
        return "Constant";
    else
        return "Identifier";
}

class Lexical
{
    static int lineNo;
    static vector<vector<Lexeme>> lexemes;
    static vector<int> scope;

public:
    static void Analyze()
    {
        File file("Processed.txt");
        file.fileCreate("Separated.txt");
        file.fileCreate("Token.txt");

        for (string line : file.readLine())
        {
            separation(line);
            validation();
            tokenGenerate();

            lineNo++;
        }

        SymbolTable::displayVariableTable();
        SymbolTable::displayFunctionTable();
    }

    static void classification(vector<string> words)
    {
        vector<Lexeme> lexeme;
        for (string word : words)
            lexeme.push_back(Lexeme(word, wordType(word)));
        lexemes.push_back(lexeme);
    }

    static void separation(string line)
    {
        File file("Separated.txt");

        spacing(line);
        file.writeLine(line, "Separated.txt");

        classification(splitLexeme(line));
    }

    static bool isSpecialSymbolExist(int lexemeNo)
    {
        for (char c : lexemes[lineNo][lexemeNo].name)
            if (!isdigit(c) && !isalpha(c) && c != '_')
                return true;
        return false;
    }

    static void validation()
    {
        for (int lexemeNo = 0; lexemeNo < lexemes[lineNo].size(); lexemeNo++)
            if (lexemes[lineNo][lexemeNo].type == "Identifier")
                if (isdigit(lexemes[lineNo][lexemeNo].name[0]))
                    ErrorHandler::pushError((lineNo + 1), "Lexical", 1, lexemes[lineNo][lexemeNo].name);
                else if (isSpecialSymbolExist(lexemeNo))
                    ErrorHandler::pushError((Lexical::lineNo + 1), "Lexical", 2, lexemes[lineNo][lexemeNo].name);

        ErrorHandler::report();
    }

    static int searchVariable(int lexemeNo)
    {
        int id = 0;
        for (VariableToken token : SymbolTable::getVariableTable())
        {
            if (lexemes[lineNo][lexemeNo].name == token.name && scope.back() == token.scope)
                return id;
            id++;
        }

        return -1;
    }

    static int searchFunction(int lexemeNo)
    {
        int id = 0;
        for (FunctionToken token : SymbolTable::getFunctionTable())
        {
            if (lexemes[lineNo][lexemeNo].name == token.name)
                return id;
            id++;
        }

        return -1;
    }

    static int getLocation(int lexemeNo, bool parameterFlag)
    {
        int location;
        if (lexemes[lineNo][lexemeNo - 1].type == "Data Type" || (lexemes[lineNo][lexemeNo - 1].name == "," && parameterFlag))
        {
            if (lexemes[lineNo][lexemeNo + 1].name == "(")
            {
                location = SymbolTable::getFunctionTable().size();
                if (scope.back() < location)
                    scope.push_back(location);
                SymbolTable::pushFunction(lexemes[lineNo][lexemeNo].name);
            }
            else
            {
                location = SymbolTable::getVariableTable().size();
                SymbolTable::pushVariable(lexemes[lineNo][lexemeNo].name, scope.back());
            }
        }
        else
        {
            location = ((lexemes[lineNo][lexemeNo + 1].name == "(") ? searchFunction(lexemeNo) : searchVariable(lexemeNo));

            if (location >= 0)
                return location;

            if (lexemes[lineNo][lexemeNo + 1].name == "(")
            {
                SymbolTable::pushFunction(lexemes[lineNo][lexemeNo].name);
                location = SymbolTable::getFunctionTable().size() - 1;
            }
            else
            {
                SymbolTable::pushVariable(lexemes[lineNo][lexemeNo].name, scope.back());
                location = SymbolTable::getVariableTable().size() - 1;
            }
        }

        vector<FunctionToken> ft = SymbolTable::getFunctionTable();
        return location;
    }

    static void tokenGenerate()
    {
        File file("Token.txt");
        bool parameterFlag = true;
        for (int lexemeNo = 0; lexemeNo < lexemes[lineNo].size(); lexemeNo++)
            if (lexemes[lineNo][lexemeNo].name == "(")
            {
                parameterFlag = false;
                file.writeWord("<(>");
            }
            else if (lexemes[lineNo][lexemeNo].name == ")")
            {
                parameterFlag = true;
                file.writeWord("<)>");
            }
            else if (lexemes[lineNo][lexemeNo].type == "Identifier")
            {
                int location = getLocation(lexemeNo, parameterFlag);
                (lexemes[lineNo][lexemeNo + 1].name == "(") ? file.writeWord(SymbolTable::getFunctionTable()[location].getToken()) : file.writeWord(SymbolTable::getVariableTable()[location].getToken());
            }
            else if (lexemes[lineNo][lexemeNo].name == "}")
            {
                scope.pop_back();
                file.writeWord("<}>");
            }
            else
                file.writeWord("<" + lexemes[lineNo][lexemeNo].name + ">");
        file.writeLine("");
    }
};

int Lexical::lineNo = 0;
vector<vector<Lexeme>> Lexical::lexemes;
vector<int> Lexical::scope = {-1};