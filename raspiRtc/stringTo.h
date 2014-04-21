#ifndef STRING_TO_H
#define STRING_TO_H

#include <string>
//#include <sstream>
using namespace std;

//int toInt(const string &s);
//double toDouble(const string &s);
//bool toBool(const string &s);

template<typename T>
T stringTo(const string &s)
{
    T ret;
    stringstream(s) >> ret;
    return ret;
}

template <class myType> string toString(myType item)
{
    ostringstream oss;
    oss << item;
    return (oss.str());
}

#endif
