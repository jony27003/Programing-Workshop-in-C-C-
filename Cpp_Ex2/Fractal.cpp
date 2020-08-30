#include <cmath>
#include <iostream>
#include <array>
#include "Fractal.h"

using std::string;
using std::cerr;
using std::cout;
using std::vector;
using std::make_pair;
using std::endl;
using std::array;
using std::exception;
using std::nothrow;




/**
* @brief Fractal constructor
* @param dimension the dimension
* @param length the length
*/
Fractal::Fractal(int dim, int grid) : _dimension(dim), _length(grid)
{
    if (this->_dimension < MIN_DIMENSION || this->_dimension > MAX_DIMENSION)
    {
        cerr << INPUT_ERROR;
        exit(EXIT_FAILURE);
    }
}


/** @brief paints a constructed fractal */
void Fractal::paintFractal()
{
    vector<string> fractalVec((unsigned long)pow(this->getLength(), this->getDimension()), "");
    create(fractalVec, this->getDimension(), 0, false);
    for(const string &str : fractalVec)
    {
        cout << str << endl;
    }
}


/**
* @brief creates a valid fractal
* @param fractalVec reference to a vector that represents the fractal
* @param dimension the dimension
* @param fractalLine the line in the fractal that is being constructed
* @param blankFlag a flag that is true iff the method is working on a blank space
*/
void Fractal::create(vector<string> &fractalVec, int dimension, int fractalLine, bool blankFlag)
{
    if (!dimension)
    {
        (blankFlag) ? fractalVec[fractalLine] += " " : fractalVec[fractalLine] += "#";
        return;
    }
    int m = 0;
    while (m < this->getLength())
    {
        int n = 0;
        while (n < this->getLength())
        {
            int target = fractalLine * this->getLength() + m;
            int nextDimension = dimension - 1;
            bool findSpace = blankFlag ||
                             (_blankCoordinates.find(make_pair(m, n)) != _blankCoordinates.end());
            create(fractalVec, nextDimension, target, findSpace);
            ++n;
        }
        ++m;
    }
}




/**
* @brief construct Fractals
* @param id fractal id (1-numberOfFractals)
* @param dimension the fractals dimension
* @return a Fractal pointer to the constructed fractal
*/
Fractal* Factory::constructFractal(int &id, int &dimension)
{
    {
        if(id == SIERPINSKI_CARPET_ID)
        {
            return new(nothrow) SierpinskiCarpetFractal(dimension);
        }
        else if(id == SIERPINSKI_SIEVE_ID)
        {
            return new(nothrow) SierpinskiSieveFractal(dimension);
        }
        else if(id == CANTOR_DUST_ID)
        {
            return new(nothrow) CantorDustFractal(dimension);
        }
        return nullptr;
    }
}




/**
* @brief Fractal constructor
* @param dimension the dimension
*/
SierpinskiCarpetFractal::SierpinskiCarpetFractal(int &dimension):
Fractal(dimension, FRACTAL_1_LENGTH)
{
    try
    {
        this->_blankCoordinates.clear();
        this->_blankCoordinates.insert(make_pair(1, 1));
    }
    catch (exception &e)
    {
        cerr << MEMORY_ERROR;
        exit(EXIT_FAILURE);
    }
}




/**
* @brief Fractal constructor
* @param dimension the dimension
*/
SierpinskiSieveFractal::SierpinskiSieveFractal(int &dimension):
Fractal(dimension, FRACTAL_2_LENGTH)
{
    try
    {
        this->_blankCoordinates.clear();
        this->_blankCoordinates.insert(make_pair(1, 1));
    }
    catch (exception &e)
    {
        cerr << MEMORY_ERROR;
        exit(EXIT_FAILURE);
    }
}




/**
* @brief Fractal constructor
* @param dimension the dimension
*/
CantorDustFractal::CantorDustFractal(int &dimension):
Fractal(dimension, FRACTAL_3_LENGTH)
{
    try
    {
        this->_blankCoordinates.clear();
        this->_blankCoordinates.insert(make_pair(0, 1));
        this->_blankCoordinates.insert(make_pair(1, 0));
        this->_blankCoordinates.insert(make_pair(1, 1));
        this->_blankCoordinates.insert(make_pair(2, 1));
        this->_blankCoordinates.insert(make_pair(1, 2));
    }
    catch (exception &e)
    {
        cerr << MEMORY_ERROR;
        exit(EXIT_FAILURE);
    }
}