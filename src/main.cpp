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

    // Step 1.1 Proccess rules to determine if it is LL or SLR
    LL ll(grammar);
    ll.first();
    ll.follow();
    ll.makeTable();
    ll.printTable();
    ll.checkString("adbc");
    ll.checkString("aabbcc");
    ll.checkString("aabcc");

    if (ll.isLL1())
    {
        cout << "The grammar is LL(1)" << endl;
    }
    else
    {
        cout << "The grammar is not LL(1)" << endl;
    }

    SLR slr(grammar);
    slr.augmentedGrammar();
    slr.createStates(vector<pair<string, string>>{make_pair("S'", ".S")});
    slr.first();
    slr.follow();
    slr.makeTable();
    slr.printTable();
    slr.checkString("zbzaz");
    slr.checkString("zaz");
    slr.checkString("zzzzz");

    // Step 2 Check if the grammar is SLR

    // Step 3 Check if the grammar is LL or SLR
    // if (slr.isSLR1() && ll.isLL1())
    // {
    //     cout << "The Grammar is a SLR and LL Type" << endl;
    // }
    // else if (slr.isSLR1()) {
    //     cout << "The Grammar is a SLR Type" << endl;
    // } else if (ll.isLL1()) {
    //     cout << "The Grammar is a LL Type" << endl;
    // } else {
    //     cout << "The grammar is neither a SLR Type nor a LL type" << endl;
    // }
    return 0;
}