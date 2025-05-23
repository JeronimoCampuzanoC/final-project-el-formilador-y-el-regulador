#include "LL.h"
#include "Node.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <stack>

#include <fstream>
#include <vector>

#include <cstdint>
using namespace std;

LL::LL(const Grammar &grammar1) : grammar(grammar1)
{
}

void LL::first()
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

void LL::follow()
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
}

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
        for (const auto &p : firstSet)
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

        if (numProductions == 1)
        {
            for (int j = 0; j < grammar.getRules().size(); j++)
            {
                // Check if the rule's left side matches the current non-terminal
                if (grammar.getRules()[j].first == nonTerminal)
                {
                    for (int k = 0; k < firstSetOfNonTerminal.size(); k++)
                    {
                        LLTable[noTerminals[nonTerminal]][terminals[firstSetOfNonTerminal[k]]] = grammar.getRules()[j].second;
                    }
                }
            }
            // iterate over the first set of the current non-terminal (we assume that if a production has epsilon, it have more than one production)
        }
        // If there is more than one prodution
        else
        {
            int elementOfSet = 0;

            for (int j = 0; j < grammar.getRules().size(); j++)
            {
                if (firstSetOfNonTerminal[elementOfSet] == "e" && grammar.getRules()[j].first == nonTerminal)
                {
                    // If the first set contains epsilon, add the follow set to the table
                    vector<string> followSetOfNonTerminal;
                    for (const auto &p : followSet)
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
                // Check if the rule's left side matches the current non-terminal
                else if (grammar.getRules()[j].first == nonTerminal)
                {
                    LLTable[noTerminals[nonTerminal]][terminals[firstSetOfNonTerminal[elementOfSet]]] = grammar.getRules()[j].second;
                    elementOfSet++;
                }
            }
        }
    }
}

void LL::printTable()
{
    cout << "LLTable" << endl;

    // Print column headers
    cout << setw(4) << " ";

    for (int j = 0; j < terminals.size(); j++)
    {

        if (j == grammar.getTerminals().size())
        {
            cout << setw(4) << "$";
            break;
        }
        else
        {
            cout << setw(4) << grammar.getTerminals()[j];
        }
    }
    cout << endl;

    for (int i = 0; i < noTerminals.size(); i++)
    {
        // Print row header
        cout << setw(4) << grammar.getNoTerminals()[i];

        // Print the table contents
        for (int j = 0; j < terminals.size(); j++)
        {
            cout << setw(4) << LLTable[i][j];
        }
        cout << endl;
    }
}

void LL::checkString(string str)
{
    // Read the string and verify if it has valid symbols
    cout << endl;

    for (int i = 0; i < str.size(); i++)
    {
        if (terminals.find(string(1, str[i])) == terminals.end())
        {
            cout << "Invalid input symbol: " << str[i] << endl;
            return;
        }
    }

    Node *root = new Node("S");

    // Add the end-of-input symbol "$" to the string
    str += "$";
    // Initialize the stack with the start symbol and the end-of-input symbol
    stack<string> parsingStack;
    parsingStack.push("$");
    parsingStack.push("S");
    string currentStackElement;
    string currentStringElement;
    string ruleApplied;
    bool success = true;

    cout << left << setw(20) << "Current string" << "| " << "Current stack" << endl;
    cout << string(20, '-') << "+----------------" << endl;
    string currentString = str;

    cout << left << setw(20) << currentString << "| ";

    stack<string> tempStack = parsingStack;
    vector<string> stackContents;

    while (!tempStack.empty())
    {
        stackContents.push_back(tempStack.top());
        tempStack.pop();
    }

    for (const string &s : stackContents)
    {
        cout << s << " ";
    }
    cout << endl;

    // Iterate over the string
    for (int i = 0; i < str.size(); i++)
    {
        currentStringElement = string(1, str[i]);
        do
        {
            currentStackElement = parsingStack.top();
            if (islower(currentStackElement[0]) || currentStackElement == "$")
            {
                break;
            }
            else
            {
                // Apply the rule at the coordinates defined by the current stack element and the current string element
                ruleApplied = LLTable[noTerminals[currentStackElement]][terminals[currentStringElement]];
                parsingStack.pop();

                // Exceptions
                if (ruleApplied == "")
                {
                    success = false;
                    i = str.size();
                    break;
                }

                // Search which node is the nontermnal that we are looking for
                Node *result = findLeftmostLeafWithValue(root, currentStackElement);
                // Create a node for each of the productions
                vector<Node *> nodes;
                for (int r = 0; r < ruleApplied.size(); r++)
                {
                    Node *newNode = new Node(string(1, ruleApplied[r]));
                    nodes.push_back(newNode);
                }

                result->children = nodes;

                for (int j = ruleApplied.size() - 1; j >= 0; j--)
                {
                    if (ruleApplied[j] != 'e')
                    {
                        parsingStack.push(string(1, ruleApplied[j]));
                    }
                }

                cout << left << setw(20) << currentString << "| ";

                tempStack = parsingStack;
                vector<string> stackContents;

                while (!tempStack.empty())
                {
                    stackContents.push_back(tempStack.top());
                    tempStack.pop();
                }

                for (const string &s : stackContents)
                {
                    cout << s << " ";
                }
                cout << endl;
            }

        } while (true && success);
        parsingStack.pop();

        cout << left << setw(20) << currentString << "| ";

        tempStack = parsingStack;
        vector<string> stackContents;

        while (!tempStack.empty())
        {
            stackContents.push_back(tempStack.top());
            tempStack.pop();
        }

        for (const string &s : stackContents)
        {
            cout << s << " ";
        }
        cout << endl;

        if (!currentString.empty())
        {
            currentString.erase(0, 1);
        }
    }

    if (!parsingStack.empty())
    {
        success = false;
    }
    if (success)
    {
        cout << "The string is valid" << endl << endl;
        // proccess to print tree
        ofstream out("./images/n_ary_tree.dot");
        out << "digraph G {\n";
        exportDot(root, out);
        out << "}\n";
        out.close();
        int result = system("dot -Tpng ./images/n_ary_tree.dot -o ./images/tree.png");

        if (result == 0)
        {
            cout << "tree.png successfully generated!" << endl <<endl;
            if (system("grep -qi microsoft /proc/version") == 0) {
                // WSL: open with Windows default image viewer
                system("powershell.exe /c start $(wslpath -w ./images/tree.png)");
            }
        }
        else
        {
            cerr << "Failed to generate PNG. Make sure Graphviz is installed and 'dot' is in your PATH." << std::endl;
        }
    }
    else
    {
        cout << "The string is not valid" << endl << endl;
    }
    cout << endl;
}

bool LL::isLL1()
{
    // Check if the grammar is not left-recursive
    for (int i = 0; i < grammar.getNoTerminals().size(); i++)
    {
        string noTerminal = grammar.getNoTerminals()[i];
        for (int j = 0; j < grammar.getRules().size(); j++)
        {
            if (grammar.getRules()[j].first == noTerminal && grammar.getRules()[j].second[0] == noTerminal[0])
            {
                cout << "Left recursion detected in the grammar. Not LL" << endl;
                return false;
            }
        }
    }
    // Check if the grammar is not ambiguous, so check if there are more duplicates in the first set
    for (int i = 0; i < firstSet.size(); i++)
    {
        for (int j = 0; j < firstSet[i].second.size(); j++)
        {
            for (int k = j + 1; k < firstSet[i].second.size(); k++)
            {
                if (firstSet[i].second[j] == firstSet[i].second[k])
                {
                    cout << "Ambiguity detected in the grammar. Not LL" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

void LL::exportDot(Node *node, ofstream &out, const string &parentId)
{
    if (!node)
        return;

    string nodeId = "\"" + node->value + "_" + to_string(reinterpret_cast<uintptr_t>(node)) + "\"";
    out << nodeId << " [label=\"" << node->value << "\"];\n";

    if (!parentId.empty())
    {
        out << parentId << " -> " << nodeId << ";\n";
    }

    for (Node *child : node->children)
    {
        exportDot(child, out, nodeId);
    }
}

Node *LL::findLeftmostLeafWithValue(Node *root, const string &target)
{
    if (!root)
        return nullptr;

    // Si el nodo coincide con el valor y no tiene hijos, es el que buscamos
    if (root->value == target && root->children.empty())
    {
        return root;
    }

    // Buscar recursivamente en los hijos en orden
    for (Node *child : root->children)
    {
        Node *result = findLeftmostLeafWithValue(child, target);
        if (result)
            return result;
    }

    return nullptr; // No encontrado
}