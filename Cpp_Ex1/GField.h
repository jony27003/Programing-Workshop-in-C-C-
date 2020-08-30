#ifndef GFIELD_H
#define GFIELD_H
#include <iostream>
#include <cmath>
class GFNumber;




/**
 * @brief GField: A class that represents a Galois field.
 */
class GField
{
private:
    /** p */
    long _p;
    /** l */
    long _l;


public:
    /**
     * @brief a constructor
     * this can be used as a default constructor or as a non default constructor.
     * @param p a given p
     * @param l a given l
     */
    GField(long p = 2, long l = 1);


    /**
     * @brief a copy constructor
     * @param other a given field
     */
    GField(const GField& other): GField(other._p, other._l){};


    /**
     * @brief a destructor.
     */
    ~GField(){};


    /**
     * @brief returns p value
     * @return the p value
     */
    long getChar() const {return this->_p; }


    /**
     * @brief returns l value
     * @return the l value
     */
    long getDegree() const {return _l; }


    /**
     * @brief returns the order of the field
     * @return the order of the field
     */
    long getOrder() const {return (long)pow(_p, _l); }


    /**
     * @brief checks is the given number is a prime number
     * @param number the number to check
     * @return true if prime, false if not
     */
    bool static isPrime(long number);


    /**
     * @brief finds the gcd of the 2 numbers
     * @param a first number
     * @param a second number
     * @return the gcd
     */
    GFNumber gcd(const GFNumber &a, const GFNumber &b) const;


    /**
     * @brief creats the number k in this field
     * @param k the number to create
     * @return a GNumber that represents k
     */
    GFNumber createNumber(long k);


    /**
     * @brief operator overload
     */
    GField& operator=(const GField& other);


    /**
     * @brief operator overload
     */
    bool operator==(const GField& other) const;


    /**
     * @brief operator overload
     */
    bool operator!=(const GField& other) const {return !(*this == other); }


    /**
     * @brief operator overload
     */
    friend std::ostream& operator<<(std::ostream& stream, const GField& field);


    /**
     * @brief operator overload
     */
    friend std::istream& operator>>(std::istream& stream, GField& field);
};




#include "GFNumber.h"
#endif