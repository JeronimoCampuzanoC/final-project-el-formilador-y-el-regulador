#include "LL.h"

#include <iostream>
#include <string>

using namespace std;

LL::LL(const Grammar& grammar1) : grammar(grammar1) {
}

void LL::first() {
    // Iterate over the noTerminals in the grammar
    for (int i = grammar.getNoTerminals().size()-1; i >= 0; i--)
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
                vector <string> secondComp;
                // Check if the first character of the production is a Noterminal
                if (isupper(production[0]))
                {
                    for (const auto& p : firstSet) {
                        if (p.first == string(1, production[0])) {
                            secondComp = p.second;
                            break;
                        }
                    }
                    for (const auto& elem : secondComp) {
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


void LL::follow() {
    for (int i = 0; i < grammar.getNoTerminals().size(); i++)
    {
        vector<string> individualFollowSet;
        string noTerminal = grammar.getNoTerminals()[i];

        if (noTerminal == "S") {
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
                        vector <string> soonToFollow;
                        for (int u = 0; u < firstSet.size(); u++) {
                            if (firstSet[u].first == string(1, production[k+1])) {
                                soonToFollow = firstSet[u].second;
                                break;  
                            }
                        }

                        // Check if the first character of the production is e

                        for (int o = 0; o < soonToFollow.size() ; o++)
                        {
                            if (soonToFollow[o] == "e") {
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
                                    vector <string> soonToFollow2;
                                    for (int u = 0; u < firstSet.size(); u++) {
                                        if (firstSet[u].first == string(1, production[k+2])) {
                                            soonToFollow2 = firstSet[u].second;
                                            break;  
                                        }
                                    }
                                    
                                    for (const auto& elem : soonToFollow2) {
                                        individualFollowSet.push_back(elem);
                                    }
                                }
                                else if ((k + 2) == production.size()){
                                    vector <string> secondComp;
                                    for (const auto& p : followSet) {
                                        if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal)) {
                                            secondComp = p.second;
                                            break;
                                        }
                                    }
                                    for (const auto& elem : secondComp) {
                                        individualFollowSet.push_back(elem);
                                    }
                                }
                            }
                        }
                    
                    }
                    else if ((k + 1) == production.size()){
                        vector <string> secondComp;
                        for (const auto& p : followSet) {
                            if (p.first == grammar.getRules()[j].first && !(p.first == noTerminal)) {
                                secondComp = p.second;
                                break;
                            }
                        }
                        for (const auto& elem : secondComp) {
                            individualFollowSet.push_back(elem);
                        }
                    }
                }
            
            }
            // If followset is not empty, add it to the followSet vector
            if (individualFollowSet.size() > 0)                
            {
                bool found = false;
                //search noTerminal in followSet
                for (int e = 0; e < followSet.size(); e++) {
                    if (followSet[e].first == noTerminal) {
                        for (int a = 0; a < individualFollowSet.size(); a++)
                        {
                            followSet[e].second.push_back(individualFollowSet[a]);
                        }
                        found = true;
                        break;
                    }
                }
                if(found == false) {
                    followSet.push_back(make_pair(noTerminal, individualFollowSet));
                }
            }
            

            
            // clear followSet for the next iteration
            individualFollowSet.clear();
            // S $
            // S abd
        }
    }
    // S   $       A   b
}


/*
    a   b   c   d   $
S  []  []  []  []  []
A  []  []  []  []  []
B  []  []  []  []  []
*/

void LL::makeTable()
{
    // Map non-terminals to row indexes
    int index = 0;
    for (auto nonTerminal : grammar.getNoTerminals())
    {
        noTerminals.insert({nonTerminal, index});
        index++;
    }

    // Map terminals to column indexes
    index = 0;
    for (auto terminal : grammar.getTerminals())
    {
        terminals.insert({terminal, index});
        index++;
    }

    // Add the end-of-input symbol "$" to the terminals
    terminals.insert({"$", index++});

    // Resize the LLTable to match the number of non-terminals and terminals
    LLTable.resize(noTerminals.size(), vector<string>(terminals.size(), ""));

    
    // Iterate over Non-terminals
    for (int i = 0; i < noTerminals.size(); i++)
    {
        string nonTerminal = grammar.getNoTerminals()[i];

        // Get the first set of the current non-terminal
        vector<string> firstSetOfNonTerminal;
        for (const auto& p : firstSet)
        {
            if (p.first == nonTerminal)
            {
                firstSetOfNonTerminal = p.second;
                break;
            }
        }

        // Search the number of productions of the current non-terminal 
        int numProductions = 0;
        for (int j = 0; j < grammar.getRules().size(); j++)
        {
            if (grammar.getRules()[j].first == nonTerminal)
            {
                numProductions++;
            }
        }
        
        for (int j = 0; j < grammar.getRules().size(); j++)
        {
            // Check if the rule's left side matches the current non-terminal
            if (grammar.getRules()[j].first == nonTerminal)
            {
                if (numProductions == 1)
                {
                    // iterate over the first set of the current non-terminal (we assume that if a production has epsilon, it have more than one production)
                    for (int k = 0; k < firstSetOfNonTerminal.size(); k++)
                    {
                        LLTable[noTerminals[nonTerminal]][terminals[firstSetOfNonTerminal[k]]] = grammar.getRules()[j].second;
                    }
                        
                }
                // If there is more than one prodution
                else{
                    for (int k = 0; k < numProductions; k++)
                    {

                        if (firstSetOfNonTerminal[k] == "e")
                        {
                            // If the first set contains epsilon, add the follow set to the table
                            vector<string> followSetOfNonTerminal;
                            for (const auto& p : followSet)
                            {
                                if (p.first == nonTerminal)
                                {
                                    followSetOfNonTerminal = p.second;
                                    break;
                                }
                            }
                            // Search epsilon rule
                            

                            // Iterate over the follow set and add it to the LLTable
                            for (int l = 0; l < followSetOfNonTerminal.size(); l++)
                            {
                                LLTable[noTerminals[nonTerminal]][terminals[followSetOfNonTerminal[l]]] = "e";
                            }
                        }
                        // If the first set does not contain epsilon, add it to the LLTable with the next logic
                        else{

                            LLTable[noTerminals[nonTerminal]][terminals[firstSetOfNonTerminal[k]]] = grammar.getRules()[j].second;
                        }
                    }
                }
            }
        }
    }
}

void LL::printTable()
{
    // Print column headers
    cout << "\t";
    for (const auto &terminal : terminals)
    {
        cout << terminal.second << "\t";
    }
    cout << endl;

    // Then print the table
}


void LL::checkString(string str) {
    
}
