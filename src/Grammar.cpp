#include "Grammar.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Grammar::Grammar(vector< pair<string, vector<string> > > rules) {
    production_rules = rules;
}

void Grammar::setRules(vector< pair<string, vector<string> > > rules) {
    production_rules = rules;
}

vector< pair<string, vector<string> > > Grammar::getRules() {
    return production_rules;
}

void Grammar::printRules() {
    cout << production_rules.size() << endl;
    for (int i = 0; i < production_rules.size(); i++){
        cout << production_rules[i].first << " -> ";
        for (int j = 0; j < production_rules[i].second.size(); j++) {
            cout << production_rules[i].second[j] << " | ";
        } 
        cout << endl;
    }
}
