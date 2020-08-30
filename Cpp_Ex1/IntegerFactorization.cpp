#include <iostream>
#include "GField.h"
using std::cin;
using std::cout;
using std::endl;


/** number of Arguments */
const int gNumberOfArguments = 2;


/**
 * @brief the main method;
 */
int main()
{
    GFNumber numbers[gNumberOfArguments];
    for(GFNumber &number : numbers)
    {
        cin >> number;
    }
    cout << numbers[0] + numbers[1] << "\n";
    cout << numbers[0] - numbers[1] << "\n";
    cout << numbers[1] - numbers[0] << "\n";
    cout << numbers[0] * numbers[1] << endl;
    numbers[0].printFactors();
    numbers[1].printFactors();
    return EXIT_SUCCESS;
}