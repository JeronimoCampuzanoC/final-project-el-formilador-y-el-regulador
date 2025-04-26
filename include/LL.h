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
    LL(const Grammar& grammar);
    void makeTable();
    void first();
    void follow();
    void checkString(string str);
    

private:
    map<int, string> terminals;
    map<int, string> noTerminals;
    vector< pair<string, vector < string> > > firstSet;
    vector< pair<string, vector < string> > > followSet;

    vector < vector <string> > LLTable;
    Grammar grammar;
};



#endif 