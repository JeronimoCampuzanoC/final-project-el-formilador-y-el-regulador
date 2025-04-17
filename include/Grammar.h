#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Grammar {
public:
    Grammar(vector< pair<string, vector<string> > > rules);
    void setRules(vector< pair<string, vector<string> > >);
    vector< pair<string, vector<string> > > getRules();
    void printRules();

private:
    vector< pair<string, vector<string> > > production_rules;
    
};

#endif // GRAMMAR_H
