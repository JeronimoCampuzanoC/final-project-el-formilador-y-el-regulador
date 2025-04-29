#include "State.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

State::State(const int& name) : name(name) {
    // Constructor implementation
}
void State::addProduction(vector< pair <string, string> >production) {
    productions = production;
}
const vector<pair <string, string>>& State::getProductions() const {
    return productions;
}
