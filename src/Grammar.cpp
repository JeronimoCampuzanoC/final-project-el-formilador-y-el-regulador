#include "Grammar.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Grammar::Grammar(vector< pair<string, string> > rules) {
    production_rules = rules;
}

void Grammar::setRules(vector< pair<string, string > > rules) {
    production_rules = rules;
}

vector< pair<string, string> > Grammar::getRules() {
    return production_rules;
}

void Grammar::printRules() {
    cout << production_rules.size() << endl;
    for (int i = 0; i < production_rules.size(); i++){
        cout << production_rules[i].first << " -> " << production_rules[i].second << endl;
    }
}
