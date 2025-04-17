#include "Reader.h"
#include <iostream>
#include <fstream>

#include <vector>
#include <tuple>
#include <string>

#include <utility>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
using namespace std;

// Function that reads the file and returns a vector full of the DFAs specified in the file
Grammar Reader::readFile(string fileRoute)
{
    int numProductions;
    vector< pair<string, vector<string> > > production_rules;
    ifstream file(fileRoute.c_str());
    try
    {
        if (!file.is_open()) {
            cerr << "Failed to open file." << endl;
            throw runtime_error("File not found");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // 1. Read the number of productions
    file >> numProductions;
    string line;
    getline(file, line);

    if (numProductions <= 0) {
        cerr << "Invalid number of productions." << endl;
        return Grammar(production_rules);
    }

    // S -> As Bd Ce 

    //  pair<string, vector<string> > 
    
    for (int i = 0; i < numProductions; ++i) {
        cout << numProductions << endl;
        getline(file, line);

        string s = line;
        size_t pos = s.find(' ');
    
        // if no space found, pos == std::string::npos and we take the whole string
        string first;
        if (pos != string::npos) {
            first = s.substr(0, pos);
        } else {
            first = s;
        }

        string second;
        pos = s.find("->");
        if (pos != string::npos) {
            second = s.substr(pos + 3);
        } else {
            second = s;
        }
        
        cout << first << " -> " << second << endl;
        


        std::vector<std::string> palabras;
    
        // Creamos un istringstream a partir del string original
        std::istringstream iss(second);
        std::string palabra;
        
        // Extraemos palabra por palabra (separa por espacios por defecto)
        while (iss >> palabra) {
            palabras.push_back(palabra);
        }


        production_rules.push_back(make_pair(first, palabras));


    }
    
    
    Grammar grammar(production_rules);
    return grammar;
}