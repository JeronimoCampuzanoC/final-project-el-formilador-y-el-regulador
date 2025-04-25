#include <iostream>
using namespace std;
#include "Reader.h"
#include "Grammar.h"
#include <string>


int main() {
    // Step 1 Proccess rules in a txt file
    Reader reader;
    Grammar grammar = reader.readFile("/home/jerocrackh/dev/university/lenguajes-formales/final-project-el-formilador-y-el-regulador/src/example.txt");
    grammar.printRules();




    // Step 2 Proccess rules to determine if it is LL or SLR

    

    // grammar.determineType();

    // if (grammar.getType() == "slr") {
    //     cout << "The Grammar is a SLR Type" << endl;
    // } else if (grammar.getType() == "ll") {
    //     cout << "The Grammar is a LL Type" << endl;
    // } else {
    //     cout << "The grammar is neither a SLR Type nor a LL type" << endl;
    // }




    // Step 3 Validate strings


    cout << "Enter a string to validate: ";

    return 0;
}