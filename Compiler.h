#pragma onece

#include "Preprocessor.h"
#include "Lexical Analyzer.h"

class Compiler
{
public:
    static void Compile(string codeFile)
    {
        Preprocessor::Process(codeFile);
        Lexical::Analyze();
    }
};