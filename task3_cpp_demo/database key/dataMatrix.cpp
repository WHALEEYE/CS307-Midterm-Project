#include "dataMatrix.h"

using namespace std;

dataMatrix::dataMatrix(const vector<std::string> &keyIniVector)
{
    numMap.reset(new vector<pNumMap>);
    floatMap.reset(new vector<pFloatMap>);
    charMap.reset(new vector<pCharMap>);
    keyIni(keyIniVector);
    mapIni();
}

void dataMatrix::keyText(ostream& os)
{
    for(vector<pNormalKey>::iterator i=key.begin();i!=key.end();++i)
    {
        switch ((*i)->getKeyType())
        {
        case MAJOR:
        case NORMAL:
        case INDEX:
            os << setiosflags(ios::left) << setw(12) << (*i)->getName();
            break;
        case LINK:
            break;
        }
    }
    os << endl;
}

void dataMatrix::keyProperty(ostream &os)
{
    for(auto i=key.begin();i!=key.end();++i)
    {
        os << (*i)->getName() << " " << keyTypeToStr((*i)->getKeyType()) << " " <<
              (*i)->getRange() << " " << uniqueToStr((*i)->getUnique()) << endl;
    }
}

void dataMatrix::writeMatrix(ostream& os)
{
    switch (majorKey->getUnique())
    {
    case NUM:
        for(auto i=numMap->at(majorKey->getIndex())->begin();i!=numMap->at(majorKey->getIndex())->end();++i)
        {
            coutTerm(i->second,os);
        }
        break;
    case CHAR:
        for(auto i=floatMap->at(majorKey->getIndex())->begin();i!=floatMap->at(majorKey->getIndex())->end();++i)
        {
            coutTerm(i->second,os);
        }
        break;
    case FLOAT:
        for(auto i=charMap->at(majorKey->getIndex())->begin();i!=charMap->at(majorKey->getIndex())->end();++i)
        {
            coutTerm(i->second,os);
        }
        break;
    case NONE:
        break;
    }
}

form dataMatrix::select(const string &keyName, const string &item) const
{
    auto k=findKey(keyName);
    int num=k->getRange();
    if(num>=0)
    {
        switch (k->getUnique())
        {
        case NUM:
            return selectNum(k,strToNum(item));
            break;
        case FLOAT:
            return selectFloat(k,strToFloat(item));
            break;
        case CHAR:
            return selectChar(k,item);
            break;
        case NONE:
            break;
        }
    }
    return NULL;
}

void dataMatrix::insert(const string &Term)
{
    stringstream ss(Term);
    term t(new vector<pBaseItem>);
    string item;
    for(vector<string>::size_type i=0;i!=key.size();++i)
    {
        ss >> item;
        switch (key[i]->getKeyType())
        {
        case MAJOR:
        case NORMAL:
        case INDEX:
            t->push_back(creatNormalItem(key[i]->getUnique(),item));
            break;
        case LINK:
            break;
        }
    }
    insertIndex(t);
}

void dataMatrix::keyIni(const vector<string> &keyIniVector)
{
    stringstream ss;
    string name;
    string type;
    string range;
    string unique;

    for(vector<std::string>::const_iterator i=keyIniVector.begin();i!=keyIniVector.end();++i)
    {
        ss.clear();
        ss.str(*i);
        ss >> name >> type >> range >> unique;
        if(type=="MAJOR")
        {
            majorKey.reset(new normalKey(name,strToNum(range),MAJOR,strToUnique(unique)));
            key.push_back(majorKey);
        }
        else if(type=="INDEX")
        {
            key.push_back(pNormalKey(new normalKey(name,strToNum(range),INDEX,strToUnique(unique))));
        }
        else if(type=="NORMAL")
        {
            key.push_back(pNormalKey(new normalKey(name,strToNum(range),NORMAL,strToUnique(unique))));
        }
        else if(type=="LINK")
        {
            ;
        }
    }
}

void dataMatrix::mapIni()
{
    for(auto i=key.begin();i!=key.end();++i)
    {
        if((*i)->getKeyType()==MAJOR||(*i)->getKeyType()==INDEX)
        {
            switch ((*i)->getUnique())
            {
            case NUM:
                (*i)->setIndex(numMap->size());
                numMap->push_back(pNumMap(new NumMap));
                break;
            case FLOAT:
                (*i)->setIndex(floatMap->size());
                floatMap->push_back(pFloatMap(new FloatMap));
                break;
            case CHAR:
                (*i)->setIndex(charMap->size());
                charMap->push_back(pCharMap(new CharMap));
                break;
            case NONE:
                break;
            }
        }
    }
}

void dataMatrix::insertIndex(term t)
{
    for(vector<int>::size_type i=0;i!=key.size();++i)
    {
        int index=key[i]->getIndex();
        if(index<0)
            continue;
        switch (key[i]->getUnique())
        {
        case NUM:
            numMap->at(index)->insert(make_pair(baseGetNum(t->at(i)),t));
            break;
        case FLOAT:
            floatMap->at(index)->insert(make_pair(baseGetFloat(t->at(i)),t));
            break;
        case CHAR:
            charMap->at(index)->insert(make_pair(baseGetChar(t->at(i)),t));
            break;
        case NONE:
            break;
        }
    }
}

const shared_ptr<normalKey> &dataMatrix::findKey(const string &keyName)const
{
    for(auto i=key.begin();i!=key.end();++i)
    {
        if((*i)->getName()==keyName)
            return *i;
    }
    throw "cannot find key";
}

form dataMatrix::selectNum(const shared_ptr<normalKey> &k, long long int data)const
{
    form ret(new vector<term>);
    auto m=numMap->at(k->getIndex());
    auto p=m->equal_range(data);
    for(auto i=p.first;i!=p.second;++i)
    {
        ret->push_back((*i).second);
    }
    return ret;
}

form dataMatrix::selectFloat(const shared_ptr<normalKey> &k, double data) const
{
    form ret(new vector<term>);
    auto m=floatMap->at(k->getIndex());
    auto p=m->equal_range(data);
    for(auto i=p.first;i!=p.second;++i)
    {
        ret->push_back((*i).second);
    }
    return ret;
}

form dataMatrix::selectChar(const shared_ptr<normalKey> &k, const string& data) const
{
    form ret(new vector<term>);
     auto m=charMap->at(k->getIndex());
     auto p=m->equal_range(data);
     for(auto i=p.first;i!=p.second;++i)
     {
         ret->push_back((*i).second);
     }
     return ret;

}

void dataMatrix::coutTerm(shared_ptr<vector<shared_ptr<baseItem> > > t,ostream& os)
{
    for(vector<int>::size_type i=0;i!=key.size();++i)
    {
        switch (key[i]->getKeyType())
        {
        case MAJOR:
        case NORMAL:
        case INDEX:
            coutNormalItem(key[i]->getUnique(),t->at(i),os);
        case LINK:
            break;
        }
    }
    os << endl;
}

void dataMatrix::coutForm(shared_ptr<vector<shared_ptr<vector<shared_ptr<baseItem> > > > > f,ostream& os)
{
    for(auto i=f->begin();i!=f->end();++i)
    {
        coutTerm(*i,os);
    }
}


