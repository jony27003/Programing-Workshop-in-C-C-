#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "infi.h"




/** @brief usage error message */
#define USAGE_ERROR "Usage: NumericalAnalyzer <function number> <a> <b> <N> <x0> <h>\n"

/** @brief Invalid input message */
#define INVALID_INPUT "Invalid input\n"

/** @brief method error */
#define ERROR 0

/** @brief method SUCCESS */
#define SUCCESS 1

/** @brief in function domain */
#define NO_DOMAIN_ERROR 0

/** @brief out of function domain */
#define DOMAIN_ERROR 1

/** @brief counting base */
#define COUNTING_BASE 10

/** @brief lowest function id */
#define VALID_FUNCTION_ID_LOWER_BOUND 1

/** @brief highest function id */
#define VALID_FUNCTION_ID_UPPER_BOUND 6

/** @brief max number of decimal points allowed in a valid double */
#define MAX_DECIMAL_POINTS_ALLOWED 1




/** @brief a global variable to determine whether or not we have tried
 * to access a function from outside its domain */
int domainError = NO_DOMAIN_ERROR;

/** @brief a constant char to represent a decimal point */
const char decimalPoint = '.';




/**
 * @brief checks whether or not the given functions number is valid and
 * converts the number if possible
 * @param str the string to convert
 * @param address the address to which the function will convert the number.
 * @return 1 if successful or 0 otherwise
 */
int validFuncId(char *str, int *address);

/**
 * @brief checks whether or not the given Point Of Derivation string is valid and
 * converts the number if possible
 * @param str the string to convert
 * @param address the address to which the Point Of Derivation will be writen at.
 * @return 1 if successful or 0 otherwise
 */
int parsePointOfDerivation(char *str, int *address);

/**
 * @brief checks if a given char is a valid digit
 * @param charToCheck the char to check
 * @return return 1 if digit or 0 otherwis
 */
int charIsDigit(char charToCheck);

/**
 * @brief extracts a double number from a string if possible
 * @param str the string from which the double will be extracted if possible
 * @param address the address of the extracted double
 * @return 1 if successful or 0 otherwise
 */
int extractDoubleFromString(char *str, double *address);

/**
 * @brief the given f1 function
 * @param x x
 * @return f1(x)
 */
double func1(double x);

/**
 * @brief the given f2 function
 * @param x x
 * @return f2(x)
 */
double func2(double x);

/**
 * @brief the given f3 function
 * @param x x
 * @return f3(x)
 */
double func3(double x);

/**
 * @brief the given f4 function
 * @param x x
 * @return f4(x)
 */
double func4(double x);

/**
 * @brief the given f5 function
 * @param x x
 * @return f5(x)
 */
double func5(double x);

/**
 * @brief the given f6 function
 * @param x x
 * @return f6(x)
 */
double func6(double x);




int main(int numberOfArguments, char *argumentsArray[])
{
    if (numberOfArguments != 7)
    {
        fprintf(stderr, "%s", USAGE_ERROR);
        return EXIT_FAILURE;
    }
    int funcId, n;
    double a, b, x0, h;
    if (!validFuncId(argumentsArray[1], &funcId) ||
        extractDoubleFromString(argumentsArray[2], &a) == ERROR ||
        extractDoubleFromString(argumentsArray[3], &b) == ERROR ||
        parsePointOfDerivation(argumentsArray[4], &n) == ERROR || (a >= b) ||
        extractDoubleFromString(argumentsArray[5], &x0) == ERROR ||
        extractDoubleFromString(argumentsArray[6], &h) == ERROR)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    --funcId;
    RealFunction RealFunctions[6] = {func1, func2, func3, func4, func5, func6};
    double extractIntegral = integration(RealFunctions[funcId], a, b, n);
    double extractDerivative = derivative(RealFunctions[funcId], x0, h);
    if(domainError)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    printf("Integral: %0.5f\n", extractIntegral);
    printf("Derivative: %0.5f\n", extractDerivative);
    return EXIT_SUCCESS;
}


int validFuncId(char *str, int *address)
{
    char *charPointer;
    int Num = strtoul(str, &charPointer, COUNTING_BASE);
    if ((strlen(str) > 1 && str[0] != '0') ||
        Num < VALID_FUNCTION_ID_LOWER_BOUND || Num > VALID_FUNCTION_ID_UPPER_BOUND)
    {
        return ERROR;
    }
    *address = Num;
    return SUCCESS;
}


int parsePointOfDerivation(char *str, int *address)
{
    for(int i = 0; i < strlen(str); ++i)
    {
        if (!charIsDigit(str[i]))
        {
            return ERROR;
        }
    }
    char *charPointer;
    *address = strtoul(str, &charPointer, COUNTING_BASE);
    return SUCCESS;
}


int charIsDigit(char charToCheck)
{
    if ((charToCheck >= '0' && charToCheck <= '9'))
    {
        return SUCCESS;
    }
    return ERROR;
}


int extractDoubleFromString(char *str, double* address)
{
    int decimalPointCounter = 0;
    for(int i = 0; i < strlen(str); ++i)
    {
        if(str[i] == decimalPoint)
        {
            ++decimalPointCounter;
        }
        if (decimalPointCounter > MAX_DECIMAL_POINTS_ALLOWED)
        {
            return ERROR;
        }
    }
    int start = 0;
    if (str[0] == '-')
    {
        ++start;
    }
    for(int i = start; i < strlen(str); ++i)
    {
        if (!charIsDigit(str[i]) && str[i] != decimalPoint)
        {
            return ERROR;
        }
    }
    char *charPointer;
    double num = strtod(str, &charPointer);
    if (num == 0 && (errno != ERROR  || charPointer == str))
    {
        return ERROR;
    }
    *address = num;
    return SUCCESS;
}


double func1(double x)
{
    return ((x*x) / 4);
}


double func2(double x)
{
    if(x <= 0)
    {
        domainError = DOMAIN_ERROR;
    }
    return (-1 * pow(x, 3) + (3 * pow(x, 2)) + x - (4 * sqrt(x))) / (2 * x * sqrt(x));
}


double func3(double x)
{
    return (sin(x) * sin(x)) - (cos(x) * cos(x));
}


double func4(double x)
{
    if(cos(2 * x) == -1)
    {
        domainError = DOMAIN_ERROR;
    }
    return (sin(x)) / (1 + cos(2 * x));
}


double func5(double x)
{
    return pow(M_E, x);
}


double func6(double x)
{
    if(x == 0)
    {
        domainError = DOMAIN_ERROR;
    }
    return (sinh(2 * x)) / (M_E * pow(x, 2));
}