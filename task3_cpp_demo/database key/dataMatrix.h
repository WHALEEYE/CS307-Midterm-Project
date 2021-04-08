#ifndef DATAMATRIX_H
#define DATAMATRIX_H

#include <map>
#include "key.h"
#include "dataitem.h"
#include "functions.h"
#include "define.h"

using namespace std;

class dataMatrix
{

public:
    dataMatrix(const vector<string>& keyIniVector);
    void keyText(ostream& os=cout);
    void keyProperty(ostream& os=cout);
    void writeMatrix(ostream& os=cout);

    form select(const string& keyName,const string& item)const;
    void insert(const string& Term);
//    void alterItem(string keyName, const string& oldItem,const string& newItem);
    void coutTerm(term t,ostream& os=cout);
    void coutForm(form f,ostream& os);

private:
    void keyIni(const vector<string>& keyIniVector);
    void mapIni();

    void insertIndex(term t);

    const pNormalKey& findKey(const string& keyName)const;
    form selectNum(const pNormalKey& k,long long int data)const;
    form selectFloat(const pNormalKey& k,double data)const;
    form selectChar(const pNormalKey& k,const string& data)const;

    shared_ptr<vector<pNumMap>> numMap;
    shared_ptr<vector<pFloatMap>> floatMap;
    shared_ptr<vector<pCharMap>> charMap;

    vector<pNormalKey> key;
    pNormalKey majorKey;

};


#endif
