#include <iostream>
#include "Reader.h"
#include "Grammar.h"
#include "LL.h"
#include "SLR.h"
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint> 


using namespace std;

int main()
{
    std::cout << R"(     ▄█        ▄█             ▄█                  
    ███       ███            ███                  
    ███       ███            ███▌                 
    ███       ███            ███▌                 
    ███       ███            ███▌                 
    ███       ███            ███                  
    ███▌    ▄ ███▌    ▄      ███                  
    █████▄▄██ █████▄▄██      █▀                   
    ▀         ▀                                   
       ▄████████  ▄█          ▄████████       ▄█  
      ███    ███ ███         ███    ███      ███  
      ███    █▀  ███         ███    ███      ███▌ 
      ███        ███        ▄███▄▄▄▄██▀      ███▌ 
    ▀███████████ ███       ▀▀███▀▀▀▀▀        ███▌ 
             ███ ███       ▀███████████      ███  
       ▄█    ███ ███▌    ▄   ███    ███      ███  
     ▄████████▀  █████▄▄██   ███    ███      █▀   
                 ▀           ███    ███            
    )";
    cout << endl
         << "Welcome to the parser!" << endl;

    // Step 1 Proccess rules in a txt file and store them in grammar
    Reader reader;
    Grammar grammar = reader.readFile("/home/jerocrackh/dev/university/lenguajes-formales/final-project-el-formilador-y-el-regulador/src/example.txt");
    cout << endl
         << endl;

    // Step 2 Check if the grammar is SLR(1) or LL(1)
    // Create LL and SLR objects
    LL ll(grammar);
    SLR slr(grammar);

    // Apply the necessary methods to the SLR object
    slr.augmentedGrammar();
    slr.createStates(vector<pair<string, string>>{make_pair("S'", ".S")});
    slr.first();
    slr.follow();
    bool isSLR = slr.makeTable();
    // Apply the necessary methods to the LL object
    ll.first();
    ll.follow();
    bool isLL = ll.isLL1();
    if (isLL == true)
    {

        ll.makeTable();
    }
    cout << endl
         << endl;

    // Step 3 Start processing the strings according to the type of grammar
    if (isSLR && isLL)
    {
        // Both SLR(1) and LL(1)
        // Create a menu to select the parser
        cout << "Select a parser (T: for LL(1), B: for SLR(1), Q: quit):" << endl;
        char in;
        string input;

        do
        {
            cout << "Enter your choice: ";
            cin >> in;
            cin.ignore(); // clear newline left in buffer

            if (in == 'T')
            {
                cout << "Grammar is LL(1)." << endl;
                ll.printTable();

                // ask for a string to check until the user enters an empty string
                do
                {
                    cout << "Enter a string to check: ";
                    getline(cin, input);
                    if (!input.empty())
                    {
                        ll.checkString(input);
                    }
                } while (!input.empty());
            }
            else if (in == 'B')
            {
                cout << "Grammar is SLR(1)." << endl;
                slr.printTable();

                // ask for a string to check until the user enters an empty string
                do
                {
                    cout << "Enter a string to check: ";
                    getline(cin, input);
                    if (!input.empty())
                    {
                        slr.checkString(input);
                    }
                } while (!input.empty());
            }
            else if (in != 'Q')
            {
                cout << "Invalid option. Please try again." << endl;
            }

        } while (in != 'Q');
    }
    else if (isLL)
    {
        // Only LL(1)
        cout << "Grammar is LL(1)." << endl;
        ll.printTable();

        string input;

        // ask for a string to check until the user enters an empty string
        do
        {
            cout << "Enter a string to check: ";
            getline(cin, input);
            if (!input.empty())
            {
                ll.checkString(input);
            }
        } while (!input.empty());
    }
    else if (isSLR)
    {
        // Only SLR(1)
        cout << "Grammar is SLR(1)." << endl;
        slr.printTable();

        string input;

        // ask for a string to check until the user enters an empty string
        do
        {
            cout << "Enter a string to check: ";
            getline(cin, input);
            if (!input.empty())
            {
                slr.checkString(input);
            }
        } while (!input.empty());
    }
    else
    {
        cout << "Grammar is neither LL(1) nor SLR(1)." << endl;
    }
    return 0;
}


