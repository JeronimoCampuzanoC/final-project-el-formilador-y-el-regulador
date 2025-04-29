#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

class State {
public:
    State(const int& name);
    void addProduction(vector< pair <string, string> > production);
    const vector< pair <string, string> >& getProductions() const;

private:
    int name;
    vector< pair <string, string> > productions;
};



#endif 