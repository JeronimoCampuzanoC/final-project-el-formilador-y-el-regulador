#include "State.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

State::State(const int& name) : name(name) {
    // Constructor implementation
}
void State::addProduction(vector< pair <string, string> >production) {
    // Itterate over the productions and add them to the productions vector
    for (const auto& prod : production) {
        // Check if the production already exists
        bool exists = false;
        for (const auto& existingProd : productions) {
            if (existingProd == prod) {
                exists = true;
                break;
            }
        }
        // If it doesn't exist, add it to the productions vector
        if (!exists) {
            productions.push_back(prod);
        }
    }
}
const vector<pair <string, string>>& State::getProductions() const {
    return productions;
}
