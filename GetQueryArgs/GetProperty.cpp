

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
    exit(0);
}

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
            continue;
        }

        if (foundEquals) value += searchString[i]; else str += searchString[i];
    }
    if (value.length() > 0) cout << value;

    // Did not find requested argument.
    exit(-1);
}
