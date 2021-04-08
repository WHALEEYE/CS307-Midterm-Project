#include <iostream>
#include "table.h"
using namespace std;

int main()
{
    table t;
    t.open("text");
    t.matrix->insert("10 486432 55 10.1 grs 45");
    t.saveAs("hello");
    t.open("hello");
    t.matrix->keyText();
    t.matrix->writeMatrix();
}
