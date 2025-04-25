#include "Grammar.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Grammar::Grammar(vector< pair<string, string> > rules) {
    production_rules = rules;
    for (int i = 0; i < rules.size(); i++){
        bool found = false;
        for (int j = 0; j < noTerminals.size(); j++){
            if (noTerminals[j] == rules[i].first){
                found = true;
                break;
            }
        }
        if (!found){
            noTerminals.push_back(rules[i].first);
        }
        found = false;
    }
}

void Grammar::setRules(vector< pair<string, string > > rules) {
    production_rules = rules;
}

vector< pair<string, string> > Grammar::getRules() {
    return production_rules;
}

vector<string> Grammar::getNoTerminals() {
    return noTerminals;
}

void Grammar::printRules() {
    cout << production_rules.size() << endl;
    for (int i = 0; i < production_rules.size(); i++){
        cout << production_rules[i].first << " -> " << production_rules[i].second << endl;
    }
}
