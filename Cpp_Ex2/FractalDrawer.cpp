#include <iostream>
#include "boost/filesystem.hpp"
#include "Fractal.h"
#include <cmath>

#define USAGE_ERROR "Usage: FractalDrawer <file path>\n"
#define INPUT_ERROR "Invalid input\n"
#define MEMORY_ERROR "Memory allocation failed\n"
#define MAX_DIMENSION 6
#define MIN_DIMENSION 1
#define NUMBER_OF_FRACTALS 3
#define MIN_NUMBER_OF_FRACTALS 1
#define COMMA ','
#define NUMBER_OF_INPUT_ARGUMENTS 2

using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::stack;
using namespace boost::filesystem;

/** @brief indicates if there was a Memory Allocation Error */
bool gMemoryAllocationErrorIndicator = false;

/**
 * @brief iff the input string is valid this method will convert the relevant
 * strings to ints and will assign them to number, and dimension
 * @param str the string to convert
 * @param number the number
 * @param dimension the dimension
 * @return true if successful or false otherwise
 */
bool isValidString(const string &str, int &number, int &dimension);

/**
 * @brief iff the input string is valid this method will load the given stack with fractals
 * @param fileName the file to check
 * @param the stack to load
 * @return true if successful or false otherwise
 */
bool isValidFile(const string &fileName, stack<Fractal*> &fractalStack);

/**
 * @brief check if there is 1 and only 1 comma in the string that is not in the edge of it
 * @param str the string to check
 * @return true if true, false if false
 */
bool validComma(const string &str);

/**
 * @brief converts a string to int
 * @param str the string to convert
 * @param number the number
 */
void stringToInt(const string &str, int &number);

/**
 * @brief checks if a given char is a digit
 * @param ch the char to check
 * @return true if successful or false otherwise
 */
bool isDigit(char ch);

/**
 * @brief free a stack of fractals
 * @param fractalStack the stack to free
 * @param toDraw if true the program will also print the fractals
 */
void freeStack(stack<Fractal*> &fractalStack, bool toDraw);




int main(int numberOfInputArguments, char **inputArguments)
{
    if (numberOfInputArguments != NUMBER_OF_INPUT_ARGUMENTS)
    {
        cerr << USAGE_ERROR;
        return EXIT_FAILURE;
    }
    path path(inputArguments[1]);
    stack<Fractal*> fractalStack;
    if (!exists(path) || !is_regular(path) || !isValidFile(inputArguments[1], fractalStack))
    {
        (gMemoryAllocationErrorIndicator) ? cerr << MEMORY_ERROR : cerr << INPUT_ERROR;
        return EXIT_FAILURE;
    }
    freeStack(fractalStack, true);
    return EXIT_SUCCESS;
}


/**
 * @brief free a stack of fractals
 * @param fractalStack the stack to free
 * @param toDraw if true the program will also print the fractals
 */
void freeStack(stack<Fractal*> &fractalStack, bool toDraw)
{
    while (!fractalStack.empty())
    {
        Fractal* fractalToFree = fractalStack.top();
        if(toDraw)
        {
            fractalToFree->paintFractal();
            cout << endl;
        }
        fractalStack.pop();
        delete fractalToFree;
    }
}


/**
 * @brief iff the input string is valid this method will load the given stack with fractals
 * @param fileName the file to check
 * @param the stack to load
 * @return true if successful or false otherwise
 */
bool isValidFile(const string &fileName, stack<Fractal*> &fractalStack)
{
    std::ifstream file(fileName);
    string str;
    Factory fractalFactory;
    while(getline(file, str))
    {
        int fractalId = 0;
        int dimension = 0;
        if (!isValidString(str, fractalId, dimension))
        {
            file.close();
            freeStack(fractalStack, false);
            return false;
        }
        Fractal* frac = fractalFactory.constructFractal(fractalId, dimension);
        if (!frac)
        {
            file.close();
            freeStack(fractalStack, false);
            gMemoryAllocationErrorIndicator = true;
            return false;
        }
        fractalStack.push(frac);
    }
    file.close();
    return true;
}


/**
 * @brief iff the input string is valid this method will convert the relevant
 * strings to ints and will assign them to number, and dimension
 * @param str the string to convert
 * @param number the number
 * @param dimension the dimension
 * @return true if successful or false otherwise
 */
bool isValidString(const string &str, int &number, int &dimension)
{
    if(str.empty() || !validComma(str))
    {
        return false;
    }
    string fractalNumber, fractalDimension;
    int categoryCounter = 1;
    for(char ch : str)
    {
        if(!isDigit(ch) && ch != COMMA)
        {
            return false;
        }
        if(ch == COMMA)
        {
            ++categoryCounter;
        }
        else
        {
            (categoryCounter == 1) ? fractalNumber += ch : fractalDimension += ch;
        }
    }
    if( fractalNumber[0] == '0' || fractalDimension[0] == '0' ||
        fractalNumber.empty() || fractalDimension.empty() ) //check 01 case with school
    {
        return false;
    }
    stringToInt(fractalNumber, number);
    stringToInt(fractalDimension, dimension);
    return (number <= NUMBER_OF_FRACTALS && number >= MIN_NUMBER_OF_FRACTALS &&
            dimension <= MAX_DIMENSION && dimension >= MIN_DIMENSION);
}


/**
 * @brief converts a string to int
 * @param str the string to convert
 * @param number the number
 */
void stringToInt(const string &str, int &number)
{
    int mult = (int)str.size() - 1;
    for(char ch : str)
    {
        number += (int)pow(10, mult) * (ch - '0');
        --mult;
    }
}


/**
 * @brief check if there is 1 and only 1 comma in the string that is not in the edge of it
 * @param str the string to check
 * @return true if true, false if false
 */
bool validComma(const string &str)
{
    if(str[0] == COMMA || str[str.size() - 1] == COMMA)
    {
        return false;
    }
    int numberOfCommas = 0;
    for(char ch : str)
    {
        if(ch == COMMA)
        {
            ++numberOfCommas;
        }
    }
    return (numberOfCommas == 1);
}


/**
 * @brief checks if a given char is a digit
 * @param ch the char to check
 * @return true if successful or false otherwise
 */
bool isDigit(char ch)
{
    return ('0' <= ch && ch <= '9');
}