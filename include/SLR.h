#ifndef SLR_H
#define SLR_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include "Grammar.h"
#include "State.h"

using namespace std;

class SLR
{
public:
    SLR(const Grammar &grammar);
    void augmentedGrammar();
    void createStates(vector<pair<string, string>> productions);
    void makeTable();
    void printTable();
    void first();
    void follow();
    void checkString(string str);

    vector<pair<string, string>> getAugmentedGrammar();

    bool isSLR1();

private:
    map<string, int> terminals;
    map<string, int> noTerminals;
    vector<State> states;
    vector<pair<string, vector<string>>> firstSet;
    vector<pair<string, vector<string>>> followSet;
    vector<pair<string, string>> augmented_grammar;
    vector<tuple<int, int, string>> gotoRegistry;

    vector<vector<string>> SLRTable;
    stack<string> stack;
    Grammar grammar;
};

#endif