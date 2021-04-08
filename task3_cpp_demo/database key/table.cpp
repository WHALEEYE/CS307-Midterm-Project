#include "table.h"
#include <iostream>

void table::open(const std::string &filename)
{
    ifstream fin(filename,ios::in);
    if(!fin.is_open())
        return;

    string str;
    vector<string> keyIniVector;
    this->filename=filename;
    while(getline(fin,str))
    {
        if(str=="$")
            break;
        keyIniVector.push_back(str);
    }
    matrix.reset(new dataMatrix(keyIniVector));
    while(getline(fin,str))
    {
        matrix->insert(str);
    }
}

void table::close()
{
    matrix.reset();
}

void table::saveAs(const string &filename)
{
    ofstream fout(filename,ios::out);
    matrix->keyProperty(fout);
    fout << "$" << "\n";
    matrix->writeMatrix(fout);
}
