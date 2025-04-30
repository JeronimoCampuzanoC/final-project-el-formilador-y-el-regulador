#include "Grammar.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Grammar::Grammar(vector<pair<string, string>> rules)
{
    production_rules = rules;
    bool foundTerminal = false;

    for (int i = 0; i < rules.size(); i++)
    {
        bool foundNoTerminal = false;
        for (int j = 0; j < noTerminals.size(); j++)
        {
            if (noTerminals[j] == rules[i].first)
            {
                foundNoTerminal = true;
                break;
            }
        }
        if (!foundNoTerminal)
        {
            noTerminals.push_back(rules[i].first);
        }
        foundNoTerminal = false;

        for (int l = 0; l < rules[i].second.size(); l++)
        {
            char currentChar = rules[i].second[l];

            // Only enters in the for-loop if the character is terminal
            if (islower(currentChar))
            {
                string currentCharStr(1, currentChar);
                bool foundTerminal = false;

                for (int k = 0; k < terminals.size(); k++)
                {
                    if (terminals[k] == currentCharStr)
                    {
                        foundTerminal = true;
                        break;
                    }
                }

                if (!foundTerminal)
                {
                    terminals.push_back(currentCharStr);
                }
            }
        }
    }
}

void Grammar::setRules(vector<pair<string, string>> rules)
{
    production_rules = rules;
}

vector<pair<string, string>> Grammar::getRules()
{
    return production_rules;
}

vector<string> Grammar::getTerminals()
{
    return terminals;
}

vector<string> Grammar::getNoTerminals()
{
    return noTerminals;
}

void Grammar::printRules()
{
    for (int i = 0; i < production_rules.size(); i++)
    {
        cout << production_rules[i].first << " -> " << production_rules[i].second << endl;
    }
}
