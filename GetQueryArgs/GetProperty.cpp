

//#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

void showUsage()
{
    cout << "CGI argument parser" << endl;
    cout << "Usage: " << endl;
    cout << "GetProperty [Name] [URL]" << endl;
    exit(0);
}

void replaceSpecial(string &input);

int main (int argc, char *argv[])
{
    if (argc < 2 ) showUsage();

    string targetParameter = "";
    if (argc >= 3 ) targetParameter = argv[argc-2];

    char *searchString = argv[argc-1];
    string str = "";
    string value = "";
    bool foundTarget = 0;
    bool foundEquals = 0;
    for (int i = 0; searchString[i] != 0; i++)
    {
        if (searchString[i] == '?')
        {
            str = "";
            value = "";
            foundTarget = 0;
            foundEquals = 0;
            continue;
        }
        if (searchString[i] == '&')
        {
            if (str.length() > 0)
            {
                if (str.compare(targetParameter) == 0)
                {
                    // Found parameter with no equals sign
                    if (value.length() == 0) cout << 1; else cout << value;
                    exit(0);
                }
            }
            str = "";
            value = "";
            foundTarget = 0;
            foundEquals = 0;
            continue;
        }
        if (searchString[i] == '=')
        {
            foundEquals = 1;
            value = "";
            continue;
        }

        if (foundEquals) value += searchString[i]; else str += searchString[i];
    }

    replaceSpecial(value);

    if (str.compare(targetParameter) == 0)
    {
         // Found parameter with no equals sign
        if (value.length() == 0) cout << 1; else cout << value;
        exit(0);
    }

    // Did not find requested argument.
    exit(-1);
}

void replaceSpecial(string &input)
{
    size_t position = 0;
    size_t length = 0;
    // Replace + with space
    position = input.find('+');
    while (position != string::npos)
    {
        input[position] = ' ';
        position = input.find('+');
    }

    // Replace escape characters
    position = input.find('%');
    while (position != string::npos)
    {
        //std::cout << std::endl << "Found %:" << std::endl;
        string hexCharString = input.substr(position+1, (size_t) 2);
        //string hexCharString = "0x" + input.substr(position+1, (size_t) 2);
        //std::cout << "Hex String: " << hexCharString << std::endl;
        std::stringstream ss;
        // Need to add 0x to make this work right...
        ss << std::hex << hexCharString;
        unsigned int replacementCharacter = 0;
        ss >> std::hex >> replacementCharacter;
        //std::cout << "replacement = [" << (unsigned char)replacementCharacter <<  "]" << std::endl;
        input.replace(position, (size_t) 3, 1, (unsigned char) replacementCharacter);
        position = input.find('%');
        //std::cout << input << std::endl;
    }

    return;
}
