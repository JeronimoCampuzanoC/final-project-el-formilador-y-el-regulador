#ifndef SLR_H
#define SLR_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Grammar.h"
#include "State.h"

using namespace std;

class SLR {
public:
    SLR(const Grammar& grammar);
    void augmentedGrammar();
    void createStates(int name, vector<pair<string, string>> productions);
    void makeTable();
    void printTable();
    void first();
    void follow();
    void checkString(string str);

    vector<pair<string, string>> getAugmentedGrammar();
    

private:
    map<string, int> terminals;
    map<string, int> noTerminals;
    vector<State> states;
    vector<pair<string, vector<string>>> followSet;
    vector<pair<string, string>> augmented_grammar;

    vector<vector<string>> LLTable;
    Grammar grammar;
};



#endif;