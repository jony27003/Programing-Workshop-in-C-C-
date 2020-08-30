#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




/**
 * @brief Defines an input error output.
 */
#define INPUT_ERROR -1

/**
 * @brief Defines an input success output.
 */
#define CORRECT_INPUT 0

/**
 * @brief Defines the minimum iterations allowed.
 */
#define MINIMUM_ITERATIONS 0

/**
 * @brief Defines valid bounds.
 */
#define VALID_BOUNDS 1

/**
 * @brief Defines not valid bounds.
 */
#define NOT_VALID_BOUNDS 0

/**
 * @brief Defines a successful exit code.
 */
#define SUCCESSFUL_EXIT_CODE 0

/**
 * @brief Defines the maximum length of a number(including the decimal point)
 */
#define MAX_VALUE_LENGTH 9

/**
 * @brief Defines that no more than 1 decimal points were found.
 */
#define EXTRA_DECIMAL_POINTS_FOUND 0

/**
 * @brief Defines the upper boundm of input samples.
 */
#define SAMPLES_UPPER_BOUND 100

/**
 * @brief The characters used to separate the input samples.
 */
#define INPUT_DIVIDERS " \t\r\n"

/**
 * @brief Defines the maximum line length.
 */
#define MAX_LINE_LENGTH 1024

/**
 * @bried A small number that the max the required array has to be greater than
 * such that the histogram will be printed.
 */
#define EPSILON 0.00000001




/**
 * @brief the array representing g samples
 */
double gArray[SAMPLES_UPPER_BOUND];

/**
 * @brief the array representing h samples
 */
double hArray[SAMPLES_UPPER_BOUND];

/**
 * @brief the number of samples in the g array
 */
int gSize;

/**
 * @brief the number of samples in the h array
 */
int hSize;

/**
 * @brief the number of iterations for which the convolution will be preformed
 */
int n;




/**
 * @brief parses the users input
 * @return INPUT_ERROR if the parsing was unsuccessful, CORRECT_INPUT otherwise.
 */
int parseInput();

/**
 * @brief Prints the histogram
 * @param array The histogram will be printed according to this array
 * @param arraySize the array size
 */
void histogram(double array[], int arraySize);

/**
 * @brief Prints Asterisks according to the histogram
 * @param n The number of Asterisks to be printed
 */
void printAsterisks(int n);

/**
 * @brief Returns the index of the maximum value in a given array
 * @param array The array to search
 * @param arraySize the array size
 */
int indexOfMaxValue(double *array, int arraySize);

/**
 * @brief Rounds the elements of the array to 3 digits after the decimal point
 * @param array The array to round
 * @param arraySize The size of the array
 */
void roundBeforeHistogram(double *array, int arraySize);

/**
 * @brief parameterization of the convolution bounds with respect to t.
 * @param t The index in the convolution
 * @param size The size of the array
 * @return The actual index after the parameterization.
 */
int parameterization(int t, int size);

/**
 * @brief Applies the convolution n times
 * @param resultArray the array that will contain the result of the convultion
 * @param n the number of times to apply the convolution.
 */
void nConvolutions(double *resultArray);

/**
 * @brief Applies a convolution
 * @param t The index for the convolution
 * @return The result of the convolution
 */
double singleConvolution(double array[], int t);

/**
 * @brief checks the validity of parameterized bounds
 * @param t The parameterized index for the convultion
 * @param m The actual index after the parameterization.
 * @return 1 if the bounds are valid or false otherwise.
 */
int checkBoundsForSingleConvolution(int t, int m);

/**
 * @brief Normalizes the given array
 * @param array The array to normalize
 * @param arraySize The size of the array
 */
void normalizeArray(double array[], int arraySize);

/**
 * @brief Normalizes and centers g and h arrays.
 */
void prepareInputForConvolution();

/**
 * @brief Centers the given array
 * @param array the array to center
 * @param arraySize the size of the array
 */
void centerArray(double array[], int arraySize);

/**
 * @brief Find the location of the decimal point.
 * @param charArray charArray that represents a double number
 * @return The location of the decimal point. If there is no decimal point this function will
 * returns the length of the "number", and if there is more than 1 decimal point,
 * this function will return an input error.
 */
int findDecimalPointPosition(char *charArray);

/**
 * @brief finds the arrays length.
 * @param inputArray The input Array
 * @return The length of the given input Array.
 */
int getInputArrayLength(char* inputArray);

/**
 * @brief Converts a char array to a double array.
 * @param arrayToConvert The array to Convert.
 * @return the valid double, or if it is not one - an input error
 */
int convertInputToDoubleArray(char arrayToConvert[], double convertedArray[]);


/**
 * @brief Converts a char array to a non positive double
 * @param array The char array to be converted
 * @return returns the double If the char array represents a valid double, otherwise,
 * returns input error.
 */
double extractDoubleFromString(char *array);



/**
 * @brief The main method.
 */
int main()
{
    if(parseInput() == INPUT_ERROR)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    prepareInputForConvolution();
    if(n > MINIMUM_ITERATIONS)
    {
        double resultArray[SAMPLES_UPPER_BOUND];
        nConvolutions(resultArray);
        histogram(resultArray, SAMPLES_UPPER_BOUND);
    }
    else
    {
        histogram(gArray, SAMPLES_UPPER_BOUND);
    }
    printf("\n");
    return SUCCESSFUL_EXIT_CODE;
}


int parseInput()
{
    char gInput[MAX_LINE_LENGTH];
    char hInput[MAX_LINE_LENGTH];
    char nInput[MAX_LINE_LENGTH];
    if ((fgets(gInput, MAX_LINE_LENGTH, stdin) == NULL) ||
        (fgets(hInput, MAX_LINE_LENGTH, stdin) == NULL) ||
        (fgets(nInput, MAX_LINE_LENGTH, stdin) == NULL))
    {
        return INPUT_ERROR;
    }
    gSize = getInputArrayLength(gInput);
    hSize = getInputArrayLength(hInput);
    if((gSize > SAMPLES_UPPER_BOUND) || (hSize > gSize) ||
       (convertInputToDoubleArray(gInput, gArray) == INPUT_ERROR) ||
       (convertInputToDoubleArray(hInput, hArray) == INPUT_ERROR))
    {
        return INPUT_ERROR;
    }
    strtok(nInput, "\n");
    if (findDecimalPointPosition(nInput) != strlen(nInput))
    {
        return INPUT_ERROR;
    }
    n = (int) extractDoubleFromString(nInput);
    return CORRECT_INPUT;
}


void histogram(double array[], int arraySize)
{
    if(array[indexOfMaxValue(array, arraySize)] <= EPSILON)
    {
        return;
    }
    roundBeforeHistogram(array, arraySize);
	for (int i = 0; i < arraySize; ++i)
	{
		printf("%0.3f: ", array[i]);
        printAsterisks(floor(( array[i] / array[indexOfMaxValue(array, arraySize)] ) * 20));
		printf("\n");
	}
	return;
}


void printAsterisks(int n)
{
    int i = 0;
    while(i<n)
    {
        printf("*");
        ++i;
    }
	return;
}


int indexOfMaxValue(double *array, int arraySize)
{
	int max = 0;
	for (int i = 0; i < arraySize; ++i)
	{
		if (array[i] > array[max])
		{
			max = i;
		}
	}
	return max;
}


void roundBeforeHistogram(double *array, int arraySize)
{
	for(int i = 0; i < arraySize; ++i)
	{
        array[i] = (round(array[i] * 1000)) / 1000;
	}
	return;
}


int parameterization(int t, int size)
{
	return (t + (floor(size / 2)));
}


void nConvolutions(double *resultArray)
{
	double temp[SAMPLES_UPPER_BOUND];
	memcpy(temp, gArray, sizeof(double) * SAMPLES_UPPER_BOUND);
	for (int i = 0; i < n; ++i)
	{
		int j = 0;
        int t = -ceil((double)SAMPLES_UPPER_BOUND / 2)-1;
		while(t <= floor((double)SAMPLES_UPPER_BOUND / 2)-1)
		{
                resultArray[j] = singleConvolution(temp, t);
				++j;
                ++t;
		}
		memcpy(temp, resultArray, sizeof(double) * SAMPLES_UPPER_BOUND);
	}
	return;
}


double singleConvolution(double array[], int t)
{
	double sum = 0;
    int m = -ceil((double)SAMPLES_UPPER_BOUND / 2);
    while (m <= floor((double)SAMPLES_UPPER_BOUND / 2))
    {
        if (checkBoundsForSingleConvolution(t, m))
        {
            int i = parameterization(t-m, SAMPLES_UPPER_BOUND);
            int j = parameterization(m, SAMPLES_UPPER_BOUND);
            double res = hArray[i] * array[j];
            sum = sum + res;
        }
        ++m;
    }
	return sum;
}


int checkBoundsForSingleConvolution(int t, int m)
{
    if ((parameterization(m, SAMPLES_UPPER_BOUND) >= 0) &&
        (parameterization(m, SAMPLES_UPPER_BOUND) < SAMPLES_UPPER_BOUND) &&
        (parameterization(t-m, SAMPLES_UPPER_BOUND) >= 0) &&
        (parameterization(t-m, SAMPLES_UPPER_BOUND) < SAMPLES_UPPER_BOUND))
    {
        return VALID_BOUNDS;
    }
    return NOT_VALID_BOUNDS;
}


void normalizeArray(double array[], int arraySize)
{
    double arraySum = 0;
    for (int i = 0; i < arraySize; ++i)
    {
        arraySum = arraySum + array[i];
    }
    if(arraySum > 0)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            array[i] = array[i] / arraySum;
        }
        return;
    }
    return;
}


void prepareInputForConvolution()
{
    normalizeArray(gArray, gSize);
    normalizeArray(hArray, hSize);
    centerArray(gArray, gSize);
    centerArray(hArray, hSize);
    return;
}


void centerArray(double array[], int arraySize)
{
	if (SAMPLES_UPPER_BOUND == arraySize)
	{
		return;
	}
	int start = (SAMPLES_UPPER_BOUND / 2) - (arraySize / 2);
    if(arraySize % 2 != 0)
    {
        --start;
    }
    for (int i = arraySize-1; i >= 0; --i)
    {
        array[i + start] = array[i];
        array[i] = 0;
    }
    return;
}


int findDecimalPointPosition(char *charArray)
{
	int decimalPointPosition  = 0;
	int i = 0;
	int numberOfDecimalPointsFound = 0;
	while(*charArray != '\0')
	{
		if (*charArray == '.')
		{
            numberOfDecimalPointsFound++;
            decimalPointPosition = i;
		}
		++i;
		++charArray;
	}
	if (numberOfDecimalPointsFound > 1)
	{
		return EXTRA_DECIMAL_POINTS_FOUND;
	}
	else if (numberOfDecimalPointsFound == 0)
	{
		return i;
	}
	return decimalPointPosition;
}


int getInputArrayLength(char* inputArray)
{
	char temp[MAX_LINE_LENGTH];
	strcpy(temp, inputArray);
	char *stringRepresentation = NULL;
	stringRepresentation = strtok(temp, INPUT_DIVIDERS);
	int i = 0;
	while(stringRepresentation != NULL)
	{
        stringRepresentation = strtok(NULL, INPUT_DIVIDERS);
		++i;
	}
	return i;
}


int convertInputToDoubleArray(char arrayToConvert[], double convertedArray[])
{
	char tempArrayToConvert[MAX_LINE_LENGTH];
	strcpy(tempArrayToConvert, arrayToConvert);
	char *arr = NULL;
	arr = strtok(tempArrayToConvert, INPUT_DIVIDERS);
	int i = 0;
	while(arr != NULL)
	{
		double convertedNumber = extractDoubleFromString(arr);
		if (convertedNumber == INPUT_ERROR)
		{
			return INPUT_ERROR;
		}
        convertedArray[i] = convertedNumber;
		++i;
        arr = strtok(NULL, INPUT_DIVIDERS);
	}
	return CORRECT_INPUT;
}


double extractDoubleFromString(char *array)
{
	if (strlen(array) > MAX_VALUE_LENGTH)
	{
		return INPUT_ERROR;
	}
	int decimalPointPosition = findDecimalPointPosition(array);
	if ((array[0] == '0' && decimalPointPosition != 1) || decimalPointPosition == 0)
	{
		return INPUT_ERROR;
	}
	double representedNumber = 0;
    double numberConstructorHelper = floor(pow((double) 10, (decimalPointPosition-1)));
	while(*array != '\0')
	{
		if (!(*array >= '0' && *array <= '9') && *array != '.')
		{
			return INPUT_ERROR;
		}
		else if (*array != '.')
		{
			int digit = *array - '0';
            representedNumber = representedNumber + (digit * numberConstructorHelper);
            numberConstructorHelper = numberConstructorHelper / 10;
			++array;
		}
		else
		{
			++array;
		}
	}
	return representedNumber;
}