#include <iostream>
#include "Reader.h"
#include "Grammar.h"
#include "LL.h"
#include "SLR.h"
#include <string>

using namespace std;

int main()
{
    // Step 1 Proccess rules in a txt file
    Reader reader;
    Grammar grammar = reader.readFile("/home/jerocrackh/dev/university/lenguajes-formales/final-project-el-formilador-y-el-regulador/src/example.txt");

    
    

    
    LL ll(grammar);
    SLR slr(grammar);
    // Step 2 Check if the grammar is SLR
    slr.augmentedGrammar();
    slr.createStates(vector<pair<string, string>>{make_pair("S'", ".S")});
    slr.first();
    slr.follow();
    bool isSLR = slr.makeTable();
    // Step 3 Check if the grammar is LL or SLR
    if (isSLR && ll.isLL1())
    {
        cout << "Select a parser (T: for LL(1), B: for SLR(1), Q: quit):" << endl;
    }
    else if (isSLR) {
        cout << "Grammar is SLR(1)." << endl;
        
        
        
        slr.printTable();
        slr.checkString("zbzaz");
        slr.checkString("zaz");
        slr.checkString("zzzzz");
    } else if (ll.isLL1()) {
        cout << "Grammar is LL(1)." << endl;
        
        ll.first();
        ll.follow();
        ll.makeTable();
        ll.printTable();
        ll.checkString("adbc");
        ll.checkString("aabbcc");
        ll.checkString("aabcc");
    } else {
        cout << "Grammar is neither LL(1) nor SLR(1)." << endl;
    }
    return 0;
}