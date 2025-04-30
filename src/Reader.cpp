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
    vector<pair<string, string>> production_rules;
    ifstream file(fileRoute.c_str());
    try
    {
        if (!file.is_open())
        {
            cerr << "Failed to open file." << endl;
            throw runtime_error("File not found");
        }
        // 1. Read the number of productions
        file >> numProductions;
        string line;
        getline(file, line); // burn line

        if (numProductions <= 0)
        {
            cerr << "Invalid number of productions." << endl;
            return Grammar(production_rules);
        }

        // S -> As Bd Ce

        //  pair<string,string>

        cout << "Number of productions: " << numProductions << endl;
        cout << "Productions:" << endl;
        for (int i = 0; i < numProductions; ++i)
        {
            getline(file, line);

            string s = line;
            size_t pos = s.find(' ');

            // if no space found, pos == std::string::npos and we take the whole string
            string first;
            if (pos != string::npos)
            {
                first = s.substr(0, pos);
            }
            else
            {
                first = s;
            }

            string second;
            pos = s.find("->");
            if (pos != string::npos)
            {
                second = s.substr(pos + 3);
            }
            else
            {
                second = s;
            }

            cout << first << " -> " << second << endl;

            std::vector<std::string> palabras;

            // Creamos un istringstream a partir del string original
            std::istringstream iss(second);
            std::string palabra;

            // Extraemos palabra por palabra (separa por espacios por defecto)
            while (iss >> palabra)
            {
                production_rules.push_back(make_pair(first, palabra));
            }
        }
        cout << "Productions read successfully." << endl;
        Grammar grammar(production_rules);
        return grammar;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}