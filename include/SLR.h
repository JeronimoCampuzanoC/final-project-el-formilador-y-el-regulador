#ifndef SLR_H
#define SLR_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Grammar.h"

using namespace std;

class SLR {
public:
    SLR(const Grammar& grammar);
    void augmentedGrammar();
    void createStates();
    void makeTable();
    void printTable();
    void first();
    void follow();
    void checkString(string str);
    

private:
    map<string, int> terminals;
    map<string, int> noTerminals;
    vector< pair<string, vector < string> > > firstSet;
    vector< pair<string, vector < string> > > followSet;

    vector < vector <string> > LLTable;
    Grammar grammar;
};



#endif;