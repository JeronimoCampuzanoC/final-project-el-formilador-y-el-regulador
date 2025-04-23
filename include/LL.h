#ifndef LL_H
#define LL_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Grammar.h"

using namespace std;

class LL {
public:
    LL(Grammar grammar);
    void makeTable();
    void first();
    void follow();
    void checkString(string str);
    

private:
    map<int, string> terminals;
    map<int, string> noTerminals;
    vector < vector <string> > LLTable;
    Grammar grammar;
};



#endif 