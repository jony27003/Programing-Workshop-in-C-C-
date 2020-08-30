#ifndef FRACTAL_H
#define FRACTAL_H




#include <string>
#include <vector>
#include <set>

using std::vector;
using std::string;
using std::set;
using std::pair;

#define MEMORY_ERROR "Memory allocation failed\n"
#define INPUT_ERROR "Invalid input\n"
#define FRACTAL_1_LENGTH 3
#define FRACTAL_2_LENGTH 2
#define FRACTAL_3_LENGTH 3

#define MAX_DIMENSION 6
#define MIN_DIMENSION 1
#define SIERPINSKI_CARPET_ID 1
#define SIERPINSKI_SIEVE_ID 2
#define CANTOR_DUST_ID 3




/** @brief A class that represents a fractal. */
class Fractal
{
public:
    /**
    * @brief gets the Fractal dimension
    * @return this Fractal dimension
    */
    int getDimension() { return this->_dimension; }

    /**
    * @brief gets the Fractal length
    * @return this Fractal length
    */
    int getLength() { return this->_length; }

    /** @brief paints a constructed fractal */
    void paintFractal();

protected:
    /** @brief this Fractal dimension */
    int _dimension;

    /** @brief this Fractal length */
    int _length;

    /** @brief blank Coordinates record */
    set<pair<int, int>> _blankCoordinates;

    /**
    * @brief Fractal constructor
    * @param dimension the dimension
    * @param length the length
    */
    explicit Fractal(int dim, int grid);

    /**
    * @brief creates a valid fractal
    * @param fractalVec reference to a vector that represents the fractal
    * @param dimension the dimension
    * @param fractalLine the line in the fractal that is being constructed
    * @param blankFlag a flag that is true iff the method is working on a blank space
    */
    void create(vector<string> &fractalVec, int dimension, int fractalLine, bool blankFlag);
};




/** @brief A class that represents a factory to construct Fractals. */
class Factory
{
public:
    /**
    * @brief construct Fractals
    * @param id fractal id (1-numberOfFractals)
    * @param dimension the fractals dimension
    * @return a Fractal pointer to the constructed fractal
    */
    Fractal* constructFractal(int &id, int &dimension);
};




/** @brief A class that represents a Sierpinski Carpet Fractal. */
class SierpinskiCarpetFractal: public Fractal
{
public:
    /**
    * @brief Fractal constructor
    * @param dimension the dimension
    */
    explicit SierpinskiCarpetFractal(int &dimension);

    /** @brief default destructor. */
    ~SierpinskiCarpetFractal() = default;

    /** @brief copy constructor. */
    SierpinskiCarpetFractal(const SierpinskiCarpetFractal &otherFractal) = default;

    /** @brief move constructor. */
    SierpinskiCarpetFractal(SierpinskiCarpetFractal && otherFractal) noexcept = default;


    /** @brief default operator. */
    SierpinskiCarpetFractal& operator=(const SierpinskiCarpetFractal &otherFractal) = default;

    /** @brief default move operator. */
    SierpinskiCarpetFractal& operator=(SierpinskiCarpetFractal && otherFractal) = default;
};




/** @brief A class that represents a Sierpinski Sieve Fractal. */
class SierpinskiSieveFractal: public Fractal
{
public:
    /**
    * @brief Fractal constructor
    * @param dimension the dimension
    */
    explicit SierpinskiSieveFractal(int &dimension);

    /** @brief default destructor. */
    ~SierpinskiSieveFractal() = default;

    /** @brief copy constructor. */
    SierpinskiSieveFractal(const SierpinskiSieveFractal &otherFractal) = default;

    /** @brief move constructor. */
    SierpinskiSieveFractal(SierpinskiSieveFractal && otherFractal) noexcept = default;

    /** @brief default operator. */
    SierpinskiSieveFractal& operator=(const SierpinskiSieveFractal &otherFractal) = default;


    /** @brief default move operator. */
    SierpinskiSieveFractal& operator=(SierpinskiSieveFractal && otherFractal) = default;
};




/** @brief A class that represents a Cantor Dust Fractal. */
class CantorDustFractal: public Fractal
{
public:
    /**
    * @brief Fractal constructor
    * @param dimension the dimension
    */
    explicit CantorDustFractal(int &dimension);

    /** @brief default destructor. */
    ~CantorDustFractal() = default;

    /** @brief copy constructor. */
    CantorDustFractal(const CantorDustFractal &otherFractal) = default;

    /** @brief move constructor. */
    CantorDustFractal(CantorDustFractal && otherFractal) noexcept = default;

    /** @brief default operator. */
    CantorDustFractal& operator=(const CantorDustFractal &otherFractal) = default;

    /** @brief default move operator. */
    CantorDustFractal& operator=(CantorDustFractal && otherFractal) = default;
};




#endif