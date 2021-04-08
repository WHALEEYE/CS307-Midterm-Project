#ifndef DATAITEM_H
#define DATAITEM_H


#include "functions.h"
#include "define.h"

using namespace std;

class baseItem
{
public:
    virtual ~baseItem(){}
};

template <typename T>
class Titem:public baseItem
{
public:
    Titem(const T& item):item(item){}
    const T& getItem()const{return item;}
    void setItem(const T& newItem){item=newItem;}

private:
    T item;
};

#endif  //DATAITEM_H
