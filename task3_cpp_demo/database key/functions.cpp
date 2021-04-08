#include "functions.h"

long long int strToNum(const string& s)
{
    long long int ret=0;
    for(auto i=s.begin();i!=s.end();++i)
    {
        if((*i)>='0'&&(*i)<='9')
            ret=ret*10+*i-'0';
        else
            throw "not a int";
    }
    return ret;
}

double strToFloat(const string& s)
{
    double ret=0;
    auto i=s.begin();
    while(i!=s.end()&&(*i)!='.')
    {
        if((*i)>='0'&&(*i)<='9')
        {
            ret=ret*10+*i-'0';
            ++i;
        }
        else
        {
            throw "not a float";
        }
    }
    double tmp=0.1;
    if(i!=s.end())
    {
        ++i;
    }
    while(i!=s.end())
    {
        if((*i)>='0'&&(*i)<='9')
        {
            ret+=tmp*((*i)-'0');
            tmp*=0.1;
            ++i;
        }
        else
        {
            throw "not a float";
        }
    }
    return ret;
}

Unique strToUnique(const string &s)
{
    if(s=="NONE")
        return NONE;
    else if(s=="NUM")
        return NUM;
    else if(s=="FLOAT")
        return FLOAT;
    else if(s=="CHAR")
        return CHAR;
    else
        throw "input error";
}

pBaseItem creatNormalItem(Unique uni,const string &item)
{
    switch (uni)
    {
    case NUM:
        return pNumItem(new NumItem(strToNum(item)));
    case FLOAT:
        return pFloatItem(new FloatItem(strToFloat(item)));
    case CHAR:
        return pCharItem(new CharItem(item));
    case NONE:
        break;
    }
    throw "unique error";
    return nullptr;
}

void coutNormalItem(Unique unique, shared_ptr<baseItem> baseItem,ostream& os)
{
    switch (unique)
    {
    case NUM:
        os << setiosflags(ios::left) << setw(12) << baseItemToNum(baseItem)->getItem();
        break;
    case CHAR:
        os << setiosflags(ios::left) << setw(12) << baseItemToChar(baseItem)->getItem();
        break;
    case FLOAT:
        os << setiosflags(ios::left) << setw(12) << baseItemToFloat(baseItem)->getItem();
        break;
    case NONE:
        throw "unique is NONE";
        break;
    }
}

string keyTypeToStr(const keyType &keyT)
{
    switch (keyT)
    {
    case MAJOR:
        return string("MAJOR");
    case INDEX:
        return string("INDEX");
    case NORMAL:
        return string("NORMAL");
    case LINK:
        return string("LINK");
    }
}

string uniqueToStr(const Unique &uni)
{
    switch (uni)
    {
    case NUM:
        return string("NUM");
    case FLOAT:
        return string("FLOAT");
    case CHAR:
        return string("CHAR");
    case NONE:
        return string("NONE");
    }
    return "";
}
