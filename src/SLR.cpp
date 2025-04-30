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
void SLR::createStates(vector<pair<string, string>> input)
{

    State newState(states.size());

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
    for (int i = 0; i < states.size(); i++)
    {
        State state = states[i];
        // Check if the productions of the new state are equal to the productions of the existing state
        if (state.getProductions() == newState.getProductions())
        {
            exists = true;
            int lenght = gotoRegistry.size();
            get<1>(gotoRegistry[lenght - 1]) = state.getName();
            break;
        }
    }
    // If it doesn't exist, add it to the states vector
    if (!exists && !newState.getProductions().empty())
    {
        states.push_back(newState);

        vector<pair<string, vector<pair<string, string>>>> groups;

        for (int i = 0; i < newState.getProductions().size(); i++)
        {
            pair<string, string> prodToCheck = newState.getProductions()[i]; //******* */
            size_t dotPos = prodToCheck.second.find('.');
            string result;
            if (dotPos != string::npos && dotPos + 1 < prodToCheck.second.size() && dotPos != prodToCheck.second.size() - 1)
            {
                string x1 = prodToCheck.second.substr(dotPos + 1, 1);
                // Search for x1 in groups
                bool foundx1 = false;
                for (auto &group : groups)
                {
                    if (group.first == x1)
                    {
                        // x1 already exists, push_back into the inner vector
                        group.second.push_back({newState.getProductions()[i]});
                        foundx1 = true;
                    }
                    std::cout << "group.first: " << group.first << ", x1: " << x1 << std::endl;
                }
                if (!foundx1)
                {
                    groups.push_back({x1, {{newState.getProductions()[i]}}});
                }
                
                // x1 not found, create new group
            }
        }
        for (int j = 0; j < groups.size(); j++)
        {
            for (int k = 0; k < groups[j].second.size(); k++)
            {
                // Reference to x3 for direct modification
                string &x3 = groups[j].second[k].second;

                // Find the position of the dot
                size_t dotPos = x3.find('.');

                // Only swap if '.' is found and there's a next character
                if (dotPos != string::npos && dotPos + 1 < x3.size())
                {
                    swap(x3[dotPos], x3[dotPos + 1]);
                }
            }
        }

        for (int h = 0; h < groups.size(); h++)
        {
            gotoRegistry.push_back(make_tuple(newState.getName(), states.size(), groups[h].first));

            createStates(groups[h].second);
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
    int index = 0;
    for (auto nonTerminal : grammar.getNoTerminals())
    {
        noTerminals.insert({nonTerminal, index});
        index++;
    }

    noTerminals.insert({"$", index});
    index++;

    for (auto terminal : grammar.getTerminals())
    {
        terminals.insert({terminal, index});
        index++;
    }

    SLRTable.resize(noTerminals.size() + terminals.size(), vector<string>(states.size(), ""));
}

// Print Table
void SLR::printTable()
{
    // Implementation of table printing
}

// First Set
void SLR::first()
{
    // Iterate over the noTerminals in the grammar
    for (int i = grammar.getNoTerminals().size() - 1; i >= 0; i--)
    {
        vector<string> individualFirstSet;
        string noTerminal = grammar.getNoTerminals()[i];

        // Iterate over the rules in the grammar
        for (int j = 0; j < grammar.getRules().size(); j++)
        {
            // Check if the rule's left side matches the current noTerminal
            if (grammar.getRules()[j].first == noTerminal)
            {
                string production = grammar.getRules()[j].second;
                vector<string> secondComp;
                // Check if the first character of the production is a Noterminal
                if (isupper(production[0]))
                {
                    for (const auto &p : firstSet)
                    {
                        if (p.first == string(1, production[0]))
                        {
                            secondComp = p.second;
                            break;
                        }
                    }
                    for (const auto &elem : secondComp)
                    {
                        individualFirstSet.push_back(elem);
                    }
                }

                // Check if the first character of the production is a terminal
                else
                {
                    individualFirstSet.push_back(string(1, production[0]));
                }
            }
        }
        // Introduce the noTerminal and its first set into the firstSet vector
        firstSet.push_back(make_pair(noTerminal, individualFirstSet));
    }
}

// Follow Set
void SLR::follow()
{
    for (int i = 0; i < grammar.getNoTerminals().size(); i++)
    {
        vector<string> individualFollowSet;
        string noTerminal = grammar.getNoTerminals()[i];

        if (noTerminal == "S")
        {
            individualFollowSet.push_back("$");
        }

        for (int j = 0; j < grammar.getRules().size(); j++)
        {
            string production = grammar.getRules()[j].second;
            for (int k = 0; k < production.size(); k++)
            {
                // Check if the current character is a noTerminal
                if (string(1, production[k]) == noTerminal)
                {
                    // Check if the next character is a terminal
                    if (k + 1 < production.size() && islower(production[k + 1]))
                    {
                        individualFollowSet.push_back(string(1, production[k + 1]));
                    }
                    // Check if the next character is a noTerminal
                    else if (k + 1 < production.size() && isupper(production[k + 1]))
                    {
                        vector<string> soonToFollow;
                        for (int u = 0; u < firstSet.size(); u++)
                        {
                            if (firstSet[u].first == string(1, production[k + 1]))
                            {
                                soonToFollow = firstSet[u].second;
                                break;
                            }
                        }

                        // Check if the first character of the production is e

                        for (int o = 0; o < soonToFollow.size(); o++)
                        {
                            if (soonToFollow[o] == "e")
                            {
                                soonToFollow.erase(soonToFollow.begin() + o);
                                // Introduce the remaining elements into the soonToFollow vector
                                for (int p = 0; p < soonToFollow.size(); p++)
                                {
                                    individualFollowSet.push_back(soonToFollow[p]);
                                }
                                // Check if the next character is a terminal

                                if (k + 2 < production.size() && islower(production[k + 2]))
                                {
                                    individualFollowSet.push_back(string(1, production[k + 2]));
                                }

                                // Check if the next character is a noTerminal

                                else if (k + 2 < production.size() && isupper(production[k + 2]))
                                {
                                    vector<string> soonToFollow2;
                                    for (int u = 0; u < firstSet.size(); u++)
                                    {
                                        if (firstSet[u].first == string(1, production[k + 2]))
                                        {
                                            soonToFollow2 = firstSet[u].second;
                                            break;
                                        }
                                    }

                                    for (const auto &elem : soonToFollow2)
                                    {
                                        individualFollowSet.push_back(elem);
                                    }
                                }
                                else if ((k + 2) == production.size())
                                {
                                    vector<string> secondComp;
                                    for (const auto &p : followSet)
                                    {
                                        if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal))
                                        {
                                            secondComp = p.second;
                                            break;
                                        }
                                    }
                                    for (const auto &elem : secondComp)
                                    {
                                        individualFollowSet.push_back(elem);
                                    }
                                }
                            }
                        }
                    }
                    else if ((k + 1) == production.size())
                    {
                        vector<string> secondComp;
                        for (const auto &p : followSet)
                        {
                            if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal))
                            {
                                secondComp = p.second;
                                break;
                            }
                        }
                        for (const auto &elem : secondComp)
                        {
                            individualFollowSet.push_back(elem);
                        }
                    }
                }
            }
            // If followset is not empty, add it to the followSet vector
            if (individualFollowSet.size() > 0)
            {
                bool found = false;
                // search noTerminal in followSet
                for (int e = 0; e < followSet.size(); e++)
                {
                    if (followSet[e].first == noTerminal)
                    {
                        for (int a = 0; a < individualFollowSet.size(); a++)
                        {
                            followSet[e].second.push_back(individualFollowSet[a]);
                        }
                        found = true;
                        break;
                    }
                }
                if (found == false)
                {
                    followSet.push_back(make_pair(noTerminal, individualFollowSet));
                }
            }

            // clear followSet for the next iteration
            individualFollowSet.clear();
        }
    }
}

// Check String
void SLR::checkString(string str)
{
    // Implementation of string checking
}