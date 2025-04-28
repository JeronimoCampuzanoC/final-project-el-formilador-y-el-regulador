#include <iostream> 
#include "Reader.h"
#include "Grammar.h"
#include "LL.h"
#include <string>

using namespace std;


int main() {
    // Step 1 Proccess rules in a txt file
    Reader reader;
    Grammar grammar = reader.readFile("/Users/andresperezquinchia/Universidad/assignment2-el-formilador-y-el-regulador/final-project-el-formilador-y-el-regulador/src/example.txt");

    // Step 1.1 Proccess rules to determine if it is LL or SLR
    LL ll(grammar);
    ll.first();
    ll.follow();
    ll.makeTable();
    ll.printTable();
    ll.checkString("adbc");
    ll.checkString("aabbcc");
    ll.checkString("aabcc");
    
    
    if(ll.isLL1()) {
        cout << "The grammar is LL(1)" << endl;
    } else {
        cout << "The grammar is not LL(1)" << endl;
    }
    


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


    //cout << "Enter a string to validate: ";

    return 0;
}