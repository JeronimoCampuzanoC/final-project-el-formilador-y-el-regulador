#include "SLR.h"
#include "State.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <utility>

using namespace std;

// Constructor
SLR::SLR(const Grammar &grammar) : grammar(grammar)
{
    //
}

// Augmented Grammar
void SLR::augmentedGrammar()
{
    augmented_grammar = grammar.getRules();

    // Add the augmented start rule
    augmented_grammar.insert(augmented_grammar.begin(), make_pair("S'", "S"));

    // Add a '.' at the start of each production
    for (auto &production : augmented_grammar)
    {
        production.second = "." + production.second;
    }
}

// Create States closure and goto implementation
void SLR::createStates(int name, vector<pair<string, string>> input)
{

    State newState(name);

    newState.addProduction(input);

    for (int i = 0; i < newState.getProductions().size(); i++)
    {
        string production = newState.getProductions()[i].second;
        size_t dotPos = production.find('.');
        if (dotPos != string::npos && dotPos + 1 < production.size())
        {
            string nextSymbol = production.substr(dotPos + 1, 1);
            if (isupper(nextSymbol[0]))
            {
                vector<pair<string, string>> newProductions;
                for (const auto &rule : augmented_grammar)
                {
                    if (rule.first == nextSymbol)
                    {
                        string newProduction = rule.second;
                        newProductions.push_back(make_pair(rule.first, newProduction));
                    }
                }
                for (auto &newProduction : newProductions)
                {
                    newState.addProduction({newProduction});
                }
            }
        }
    }
    // Check if the new state already exists
    bool exists = false;
    for (const auto &state : states)
    {
        if (state.getProductions() == newState.getProductions())
        {
            exists = true;
            break;
        }
    }
    // If it doesn't exist, add it to the states vector
    if (!exists)
    {
        states.push_back(newState);

        for (int i = 0; i < newState.getProductions().size(); i++) {
            vector<pair<string, string>> prodToCheck = {newState.getProductions()[i]};
            size_t dotPos = prodToCheck[0].second.find('.');
            string result;
            if (dotPos != string::npos && dotPos + 1 < prodToCheck[0].second.size() && dotPos != prodToCheck[0].second.size() - 1) {
                result = prodToCheck[0].second;
                swap(result[dotPos], result[dotPos + 1]);
            }
            prodToCheck[0].second = result;
            createStates(name + 1, prodToCheck);
        }
    }
}

    vector<pair<string, string>> SLR::getAugmentedGrammar()
    {
        return augmented_grammar;
    }

    // Make Table
    void SLR::makeTable()
    {
        // Implementation of table creation
    }

    // Print Table
    void SLR::printTable()
    {
        // Implementation of table printing
    }

    // First Set
    void SLR::first()
    {
        // Implementation of first set calculation
    }

    // Follow Set
    void SLR::follow()
    {
        // Implementation of follow set calculation
    }

    // Check String
    void SLR::checkString(string str)
    {
        // Implementation of string checking
    }
