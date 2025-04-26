#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Grammar {
public:
    Grammar();
    Grammar(vector< pair<string, string> > rules);
    void setRules(vector< pair<string, string> >);
    vector< pair<string, string> > getRules();
    vector<string> getTerminals();
    vector<string> getNoTerminals();
    void printRules();

private:
    vector< pair<string, string > > production_rules;
    vector<string> noTerminals;
    vector<string> terminals;
};

#endif // GRAMMAR_H
