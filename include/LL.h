#ifndef LL_H
#define LL_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Grammar.h"
#include "Node.h"

using namespace std;

class LL
{
public:
    LL(const Grammar &grammar);
    void makeTable();
    void printTable();
    void first();
    void follow();
    void checkString(string str);
    bool isLL1();
    Node* findLeftmostLeafWithValue(Node *root, const std::string &target);
    void exportDot(Node *node, std::ofstream &out, const std::string &parentId = "");

private:
    map<string, int> terminals;
    map<string, int> noTerminals;
    vector<pair<string, vector<string>>> firstSet;
    vector<pair<string, vector<string>>> followSet;

    vector<vector<string>> LLTable;
    Grammar grammar;
};

#endif