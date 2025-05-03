#include "SLR.h"
#include "State.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iomanip>
#include <tuple>
#include <unordered_set>
#include <algorithm>
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

    // Create a new state
    State newState(states.size());

    newState.addProduction(input);

    // Apply the closure algorithm
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
bool SLR::makeTable()
{
    try
    {
        int index = 0;

        // Fill the terminals and noTerminals maps
        for (auto terminal : grammar.getTerminals())
        {
            terminals.insert({terminal, index});
            index++;
        }

        terminals.insert({"$", index});
        index++;

        for (auto nonTerminal : grammar.getNoTerminals())
        {
            noTerminals.insert({nonTerminal, index});
            index++;
        }

        // Create the SLRTable
        SLRTable.resize(states.size(), vector<string>(noTerminals.size() + terminals.size(), ""));

        // Read all gotoRegistry, this should put number and shifts
        for (int i = 0; i < gotoRegistry.size(); i++)
        {
            int state = get<0>(gotoRegistry[i]);
            int nextState = get<1>(gotoRegistry[i]);
            string symbol = get<2>(gotoRegistry[i]);

            // Check if the symbol is a terminal or non-terminal
            if (terminals.find(symbol) != terminals.end())
            {
                SLRTable[state][terminals[symbol]] = "s" + to_string(nextState);
            }
            else
            {
                SLRTable[state][noTerminals[symbol]] = to_string(nextState);
            }
        }

        // Read all states and productions to set the accept and reduce
        for (int i = 0; i < states.size(); i++)
        {
            for (int j = 0; j < states[i].getProductions().size(); j++)
            {
                string production = states[i].getProductions()[j].second;
                size_t dotPos = production.find('.');
                if (dotPos != string::npos && dotPos == production.size() - 1)
                {
                    // Accept state
                    if (production == "S.")
                    {
                        if (SLRTable[i][terminals["$"]] != "")
                        {
                            throw std::runtime_error("Something went wrong!, this is not and slr(1) grammar");
                        }

                        SLRTable[i][terminals["$"]] = "acc";
                    }
                    else
                    {

                        pair<string, string> productionToReduce = states[i].getProductions()[j];
                        // clear the dot from the production
                        productionToReduce.second = production.substr(0, dotPos);
                        // search in grammar for the production and get the index
                        for (int k = 0; k < grammar.getRules().size(); k++)
                        {
                            if (grammar.getRules()[k].first == productionToReduce.first && grammar.getRules()[k].second == productionToReduce.second)
                            {
                                // Get the follow set for the production
                                for (int l = 0; l < followSet.size(); l++)
                                {
                                    if (followSet[l].first == productionToReduce.first)
                                    {
                                        for (int m = 0; m < followSet[l].second.size(); m++)
                                        {
                                            if (SLRTable[i][terminals[followSet[l].second[m]]] != "")
                                            {
                                                throw std::runtime_error("Something went wrong!, this is not and slr(1) grammar");
                                            }
                                            SLRTable[i][terminals[followSet[l].second[m]]] = "r" + to_string(k);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        std::cout << "Caught an exception: " << e.what() << std::endl;
        return false;
    }
}

// Print Table
void SLR::printTable()
{
    // print SRLTable
    cout << "SLRTable" << endl;
    // Print column headers
    cout << setw(4) << " ";
    for (int i = 0; i < terminals.size(); i++)
    {
        for (const auto &pair1 : terminals)
        {
            if (pair1.second == i)
            {
                std::cout << setw(4) << pair1.first;
                break;
            }
        }
    }
    for (int i = terminals.size(); i < noTerminals.size() + terminals.size(); i++)
    {
        for (const auto &pair1 : noTerminals)
        {
            if (pair1.second == i)
            {
                std::cout << setw(4) << pair1.first;
                break;
            }
        }
    }

    cout << endl;

    for (int i = 0; i < states.size(); i++)
    {
        // Print row header
        cout << setw(4) << states[i].getName();
        for (int j = 0; j < terminals.size() + noTerminals.size(); j++)
        {
            // Print the value in the table
            cout << setw(4) << SLRTable[i][j];
        }
        cout << endl;
    }

    cout << endl << endl << endl << "States" << endl;
    for (int i = 0; i < states.size(); i++)
    {
        cout << "State " << states[i].getName() << ": ";
        cout << endl;
        for (int j = 0; j < states[i].getProductions().size(); j++)
        {
            cout << setw(4) << states[i].getProductions()[j].first << " -> " << states[i].getProductions()[j].second << endl;
        }
        cout << endl;
    }
    cout << endl
         << endl;

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
                                // soonToFollow.erase(soonToFollow.begin() + o);
                                // Introduce the remaining elements into the soonToFollow vector
                                for (int p = 0; p < soonToFollow.size(); p++)
                                {
                                    // Check if the element is not already in the individualFollowSet
                                    bool found = false;
                                    for (int a = 0; a < individualFollowSet.size(); a++)
                                    {
                                        if (individualFollowSet[a] == soonToFollow[p])
                                        {
                                            found = true;
                                            break;
                                        }
                                    }
                                    // If not found, add it to the individualFollowSet
                                    if (!found)
                                    {
                                        individualFollowSet.push_back(soonToFollow[p]);
                                    }
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

                                    // Iterate through the firstSet vector to find the first set of the non-terminal at position k+2
                                    for (int u = 0; u < firstSet.size(); u++)
                                    {

                                        // If the non-terminal matches, store its first set in soonToFollow2
                                        if (firstSet[u].first == string(1, production[k + 2]))
                                        {
                                            soonToFollow2 = firstSet[u].second;
                                            break;
                                        }
                                    }

                                    // Add each element of the first set of the non-terminal to the individualFollowSet
                                    for (const auto &elem : soonToFollow2)
                                    {
                                        individualFollowSet.push_back(elem);
                                    }
                                }

                                // If the character two positions ahead is the end of the production
                                else if ((k + 2) == production.size())
                                {
                                    vector<string> secondComp;

                                    // Iterate through the followSet vector to find the follow set of the
                                    // current rule's non-terminal
                                    for (const auto &p : followSet)
                                    {
                                        if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal))
                                        {
                                            // If the left-hand side matches and is not the current non-terminal,
                                            // store its follow set in secondComp and exit the loop
                                            secondComp = p.second;
                                            break;
                                        }
                                    }

                                    // Add each element of the follow set to the individualFollowSet
                                    for (const auto &elem : secondComp)
                                    {
                                        individualFollowSet.push_back(elem);
                                    }
                                }
                            }
                        }
                    }

                    // If the current character is the last character in the production
                    else if ((k + 1) == production.size())
                    {
                        vector<string> secondComp;

                        // Iterate through the followSet to find the
                        // follow set of the current rule's non-terminal
                        for (const auto &p : followSet)
                        {

                            // If the left-hand side matches and is not the current non-terminal,
                            // then store its follow set in secondComp and exit the loop
                            if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal))
                            {
                                secondComp = p.second;
                                break;
                            }
                        }

                        // Add each element of the follow set to the individualFollowSet
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

    // erase duplicates
    for (auto &pair : followSet)
    {
        unordered_set<string> seen;
        vector<string> unique;

        for (const auto &val : pair.second)
        {
            if (seen.insert(val).second)
            { // Only insert if not already seen
                unique.push_back(val);
            }
        }

        pair.second = std::move(unique); // Replace with unique values
    }
}

// Check String
void SLR::checkString(string str)
{
    // Read the string

    string input = str;
    input += "$";
    stack<string> stack;
    stack.push("0");
    int index = 0;
    string action = "";
    string state = "";
    string top_stack = stack.top();
    string head_input = input.substr(index, 1);

    // Check if the string is empty
    if (input == "$")
    {
        cout << "The string is empty" << endl;
        cout << endl;
        return;
    }

    while (true)
    {
        top_stack = stack.top();
        head_input = input.substr(index, 1);

        // Get the action from the SLR table
        if (terminals.find(head_input) != terminals.end())
        {
            action = SLRTable[stoi(top_stack)][terminals[head_input]];
        }
        else
        {
            cout << "Invalid input symbol: " << head_input << endl;
            cout << endl;
            return;
        }

        if (action.empty())
        {
            cout << "The string is rejected" << endl;
            cout << endl;
            return;
        }

        if (action[0] == 's') // Shift
        {
            string next_state = action.substr(1);
            stack.push(head_input);
            stack.push(next_state);
            index++;
        }
        else if (action[0] == 'r') // Reduce
        {
            int rule_index = stoi(action.substr(1));
            auto rule = grammar.getRules()[rule_index];
            string lhs = rule.first;
            string rhs = rule.second;

            // Pop 2 * |rhs| elements from the stack
            for (int i = 0; i < 2 * rhs.size(); i++)
            {
                stack.pop();
            }

            top_stack = stack.top();
            stack.push(lhs);

            // Push the goto state
            if (noTerminals.find(lhs) != noTerminals.end())
            {
                stack.push(SLRTable[stoi(top_stack)][noTerminals[lhs]]);
            }
            else
            {
                cout << "Error: Invalid non-terminal in reduction" << endl;
                return;
            }
        }
        else if (action == "acc") // Accept
        {
            cout << "The string is accepted" << endl;
            cout << endl;
            return;
        }
        else
        {
            cout << "The string is rejected" << endl;
            cout << endl;
            return;
        }
    }
}
