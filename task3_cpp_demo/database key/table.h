#ifndef TABLE_H
#define TABLE_H

#include "dataMatrix.h"

class table
{
public:
    void open(const string& filename);
    void close();
    void saveAs(const string& filename);

    string filename;
    pDataMatrix matrix;
};

#endif // TABLE_H
