#include <iostream>
using namespace std;
#include "Reader.h"
#include "Grammar.h"
#include <string>


int main() {
    // Step 1 Proccess rules in a txt file
    Reader reader;
    reader.readFile("grammar.txt");
    




    // Step 2 Proccess rules to determine if it is LL or SLR

    Grammar grammar;

    // grammar.determineType();

    // if (grammar.getType() == "slr") {
    //     cout << "The Grammar is a SLR Type" << endl;
    // } else if (grammar.getType() == "ll") {
    //     cout << "The Grammar is a LL Type" << endl;
    // } else {
    //     cout << "The grammar is neither a SLR Type nor a LL type" << endl;
    // }




    // Step 3 Validate strings




    return 0;
}