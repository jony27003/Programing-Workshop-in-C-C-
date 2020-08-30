#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "boost/filesystem.hpp"
#include "HashMap.hpp"

#define USAGE_ERROR "Usage: SpamDetector <database path> <message path> <threshold>\n"
#define INPUT_ERROR "Invalid input\n"

using std::string;
using std::stringstream;
using std::ifstream;
using std::cerr;
using boost::filesystem::path;
using boost::filesystem::is_regular;
using boost::filesystem::exists;
using std::cout;
using std::istringstream;
using std::getline;
using std::count;
using std::exception;
using std::tolower;


/**
* @brief analyzes the given input
* @param inputDataBase the data base to analyze
* @param hashMap the hashMap to relocate it to
*/
void analyzeInput(const string &inputDataBase, HashMap<string, int> &hashMap);

/**
* @brief checks if a string is valid and converts it to an integer
* @param str the string to check
* @param num the number
* @param can this number Be Zero or not
* @return true if this method was successful or false otherwise
*/
bool isValidString(string &str, int &num, bool canBeZero);

/**
* @brief check if a given char is a valid digit
* @param ch the char to check
* @return true if this method was successful or false otherwise
*/
bool isDigit(char ch);

/**
* @brief converts a string to an integer
* @param str the string to convert
* @param number the number
*/
void stringToInt(const string &str, int &number);

/**
* @brief divides a string by ',' to two given strings
* @param input the input string
* @param word the word
* @param points the points of the word
* @return true if this method was successful or false otherwise
*/
bool divideString(string &input, string &word, int &points);

/**
* @brief check if the given message is spam or not
* @param hashMap the hash map
* @param message the given message
* @param bound the max number of points from witch this message is considered spam
* @return
*/
bool spam(const HashMap<string, int> &hashMap, string &message, int bound);

/**
* @brief counts the number of commas in a given string
* @param str the given string
* @return true if found one comma false otherwise
*/
bool commaCount(const string &str);

/**
* @brief converts a given string to lower case
* @param message the string to convert
*/
void convertStringToLowerCase(string &message);

/**
* @brief converts a File To String
* @param inputMessageFile the file to convert
* @param message the message
*/
void convertFileToString(string &inputMessageFile, string &message);




/**
* @brief the main function
* @param numberOfArguments the number Of Arguments given
* @param argumentsArray the given arguments Array
* @return 0 if this method was successful or 1 otherwise
*/
int main(int numberOfArguments, char* argumentsArray[])
{
    if (numberOfArguments != 4)
    {
        cerr << USAGE_ERROR;
        return EXIT_FAILURE;
    }
    int bound = 0;
    path inputDataBasePath(argumentsArray[1]);
    path inputMessagePath(argumentsArray[2]);
    string strThreshold(argumentsArray[3]);
    HashMap<string, int> hashMap;
    if (!(exists(inputDataBasePath) && is_regular(inputDataBasePath) && exists(inputMessagePath)
        && is_regular(inputMessagePath)) || !isValidString(strThreshold, bound, false))
    {
        cerr << INPUT_ERROR;
        return EXIT_FAILURE;
    }
    try
    {
        analyzeInput(argumentsArray[1], hashMap);
    }
    catch (exception & e)
    {
        cerr << INPUT_ERROR;
        return EXIT_FAILURE;
    }
    string message;
    string inputMessageFile = argumentsArray[2];
    convertFileToString(inputMessageFile, message);
    (spam(hashMap, message, bound)) ? cout << "SPAM\n" : cout << "NOT_SPAM\n";
    return EXIT_SUCCESS;
}



void convertFileToString(string &inputMessageFile, string &message)
{
    ifstream helper1(inputMessageFile);
    stringstream helper2;
    helper2 << helper1.rdbuf();
    message = helper2.str();
    helper1.close();
}


bool isValidString(string &str, int &num, bool canBeZero)
{
    if(str.empty())
    {
        return false;
    }
    int nonZeroCounter = 0;
    int zeroCounter = 0;
    string toConvert;
    for(char ch : str)
    {
        if(!isDigit(ch))
        {
            return false;
        }
        if(ch != '0' || nonZeroCounter > 0)
        {
            toConvert += ch;
            ++nonZeroCounter;
        }
        else
        {
            ++zeroCounter;
        }
    }
    if(!toConvert.empty())
    {
        stringToInt(toConvert, num);
        return  true;
    }
    return (canBeZero && zeroCounter);
}


bool isDigit(char ch)
{
    return ('0' <= ch && ch <= '9');
}


void stringToInt(const string &str, int &number)
{
    int mult = (int)str.size() - 1;
    for(char ch : str)
    {
        number += (int)pow(10, mult) * (ch - '0');
        --mult;
    }
}


void analyzeInput(const string &inputDataBase, HashMap<string, int> &hashMap)
{

    ifstream dbFile(inputDataBase);
    string line;
    while (getline(dbFile, line))
    {
        string word;
        int points = 0;
        if(!divideString(line, word, points))
        {
            dbFile.close();
            throw OutOfRange();
        }
        hashMap.insert(word, points);
    }
    dbFile.close();
}


bool divideString(string &input, string &word, int &points)
{
    if(!commaCount(input))
    {
        return false;
    }
    bool commaFlag = false;
    string strPoints;
    for(char ch : input)
    {
        if(ch == ',')
        {
            commaFlag = true;
        }
        else
        {
            (commaFlag) ? strPoints += ch : word += ch;
        }
    }
    if(word.empty() || strPoints.empty())
    {
        return false;
    }
    return (isValidString(strPoints, points, true));
}


bool commaCount(const string &str)
{
    if(str[0] == ',' || str[str.size() - 1] == ',')
    {
        return false;
    }
    int numberOfCommas = 0;
    for(char ch : str)
    {
        if(ch == ',')
        {
            ++numberOfCommas;
        }
    }
    return (numberOfCommas == 1);
}


bool spam(const HashMap<string, int> &hashMap, string &message, int bound)
{
    convertStringToLowerCase(message);

    HashMap<string, int>::const_iterator start = hashMap.cbegin();
    int points = 0;
    while (start != hashMap.cend())
    {
        string word = (*start).first;
        convertStringToLowerCase(word);
        if (message.find(word) != string::npos)
        {
            points += (*start).second;
        }
        ++start;
    }
    return (points >= bound);
}


void convertStringToLowerCase(string &message)
{
    for(char &ch : message)
    {
        ch = (char)std::tolower(ch);
    }
}