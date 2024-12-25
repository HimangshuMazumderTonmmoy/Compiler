#pragma once

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class File
{
    string directory;
    fstream file;

public:
    File(string directory = "Example.txt") : directory(directory) {}

    void changeDirectory(string directory)
    {
        this->directory = directory;
    }

    void fileOpen(ios_base::openmode mode, string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        file.open(directory, mode);
        if (!file.is_open())
        {
            cout << "File failed to open" << endl;
            return;
        }
    }

    void fileCreate(string directory = "")
    {
        if (directory == "")
            directory = this->directory;
        fileOpen(ios::out, directory);
        file.close();
    }

    void fileDelete(string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        if (remove(directory.c_str()) != 0)
            cout << "Failed to delete the file" << endl;
    }

    bool fileExist(string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        fstream file(directory);

        bool flag;

        flag = ((file) ? true : false);

        file.close();

        return flag;
    }

    void fileAppend(string copyFile, string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        for (string line : readLine(copyFile))
            writeLine(line, directory);
    }

    vector<string> readLine(string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        vector<string> lines;
        if (fileExist(directory))
        {
            fileOpen(ios::in, directory);

            string line;
            while (getline(file, line))
                if (!line.empty())
                    lines.push_back(line);

            file.close();
        }
        else
            lines.push_back("File does not exist.");

        return lines;
    }

    vector<string> readWord(string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        vector<string> words;
        if (fileExist(directory))
        {
            fileOpen(ios::in, directory);
            string word;
            while (file >> word)
                words.push_back(word);

            file.close();
        }
        else
            words.push_back("File does not exist");

        return words;
    }

    string readCharacter(string directory = "")
    {
        if (directory == "")
            directory = this->directory;

        string letters;
        if (fileExist(directory))
        {
            fileOpen(ios::in, directory);

            char letter;
            while (file.get(letter))
            {
                if (letter == ' ' || letter == '\n')
                    continue;
                letters.push_back(letter);
            }

            file.close();
        }
        else
            letters = "File does not exist";

        return letters;
    }

private:
    void write(string directory)
    {
        if (directory == "")
            directory = this->directory;

        fileOpen(ios::app, directory);
    }

public:
    void writeLine(string line, string directory = "")
    {
        write(directory);
        file << line << endl;
        file.close();
    }

    void writeWord(string line, string directory = "")
    {
        write(directory);
        file << line << " ";
        file.close();
    }
};