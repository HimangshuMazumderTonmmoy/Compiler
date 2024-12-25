#pragma once

#include <sstream>
#include "File.h"
#include "ErrorHandler.h"

vector<string> splitWord(string line)
{
    istringstream iss(line);

    string word;
    vector<string> words;

    while (iss >> word)
        words.push_back(word);

    return words;
}

string removeComment(string line)
{
    for (int i = 0; i < line.size(); i++)
        if (line[i] == '/' && line[i + 1] == '/')
            return line.substr(0, i);
    return line;
}

string removeSpaceBetween(string line)
{
    bool flag = false;
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] != ' ')
            flag = true;

        if (line[i] == ' ' && line[i + 1] == ' ' && flag)
            line.erase(line.begin() + i--);
    }
    return line;
}

string extractLibraryName(string includeLine)
{
    int index = includeLine.find('"');
    return includeLine.substr((index + 1), (includeLine.size() - index - 2));
}

class Preprocessor
{
public:
    static void Process(string retrievingFile, string writingFile = "Processed.txt")
    {
        File file(writingFile);
        file.fileCreate(writingFile);
        for (string line : file.readLine(retrievingFile))
        {
            vector<string> words = splitWord(line);
            if ((words[0] == "#include" && words[1] == "<iostream>") || (words[0] == "using" && words[1] == "namespace" && words[2] == "std;"))
                continue;
            else if (words[0] == "#include")
            {
                string library = extractLibraryName(line);
                string libraryPrepossessing = library.substr(0, (library.size() - 4)) + " Processed.txt";
                Process(library, libraryPrepossessing);
                file.fileAppend(libraryPrepossessing);
                file.fileDelete(libraryPrepossessing);
            }
            else
            {
                line = removeComment(line);
                if (!line.empty())
                    file.writeLine(removeSpaceBetween(removeComment(line)));
            }
        }
    }
};