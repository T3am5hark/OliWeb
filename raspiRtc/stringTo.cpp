#include "stringTo.h"
#include <sstream>
#include <iostream>

using namespace std;

//int toInt(const string &s);
//double toDouble(const string &s);
//bool toBool(const string &s);



int toInt(const string &s)
{
    int anInt = 0;
    stringstream(s) >> anInt;
    return anInt;
}
double toDouble(const string &s)
{
    double ret = 0.0;
    stringstream(s) >> ret;
    return ret;
}
bool toBool(const string &s)
{
    bool ret = false;
    stringstream(s) >> ret;
    return ret;
}

template<typename T>
T stringTo(const string &s)
{
    T ret;
    stringstream(s) >> ret;
    return ret;
}


//template<typename T>
//T stringTo(string s);
