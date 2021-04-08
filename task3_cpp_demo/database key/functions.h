#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <string>
#include <vector>
#include <memory>
#include <bits/stdc++.h>
#include <iostream>
#include "dataitem.h"

using std::string;
using std::shared_ptr;

class baseItem;

enum Unique
{
    NONE,
    NUM,
    FLOAT,
    CHAR
};

enum keyType
{
    MAJOR,
    INDEX,
    NORMAL,
    LINK
};

long long int strToNum(const string& s);
double strToFloat(const string& s);
Unique strToUnique(const string& s);
string keyTypeToStr(const keyType& keyT);
string uniqueToStr(const Unique& uni);
pBaseItem creatNormalItem(Unique uni,const string& item);
pBaseItem creatMajorItem(int n);
void coutNormalItem(Unique unique,pBaseItem baseItem,ostream& os);

#endif // FUNCTIONS_H
