#ifndef KEY_H
#define KEY_H

#include "functions.h"
#include "define.h"

using namespace std;

class normalKey
{
public:
    normalKey(const string& name,int range,keyType keyT=NORMAL,Unique uni=NONE)
        :name(name),range(range),keyT(keyT),unique(uni){index=-1;}
    const int& getRange()const{return range;}
    const int& getIndex()const{return index;}
    const string& getName()const{return name;}
    const Unique& getUnique()const{return unique;}
    const keyType& getKeyType()const{return keyT;}
    void setIndex(int n){index=n;}

private:
    string name;
    int range;
    int index;
    keyType keyT;
    Unique unique;
};
#endif  //KEY_H
