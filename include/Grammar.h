#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <map>

class Grammar {
public:
    void determineType();
    void printRules();

    string getType();  

private:
    string type;
    <vector<pair<string, string>>> production_rules;
    
};

#endif // GRAMMAR_H
