#include "GField.h"
#ifndef GFNUMBER_H
#define GFNUMBER_H
#include <iostream>




/**
 * @brief GFNumber: A class that represents a number in a a Galois field.
 */
class GFNumber
{
private:
    /** The field */
    GField _field;
    /** the number */
    long _n;
    /** the factorization algorithm */
    long _longFactorization(long num) const;
    /** the factorization algorithm polynom */
    long _function(long x) const {return x * x + (long)1; }
    /** a method to copy an array and update its size */
    static GFNumber* _correctArray(GFNumber *array, int &arraySize, int newArraySize);


public:
    /**
     * @brief a constructor
     * this can be used as a default constructor or as a non default constructor.
     * @param n a given number
     * @param field a given field
     */
    GFNumber(const long n = (long)0, const GField& field = GField(2, 1));


    /**
     * @brief a copy  constructor
     * @param other a given number
     */
    GFNumber(const GFNumber& other): GFNumber(other._n, other._field){}


    /**
     * @brief a destructor.
     */
    ~GFNumber(){};


    /**
     * @brief returns the number
     * @return the number
     */
    long getNumber() const {return _n; }


    /**
     * @brief returns the field
     * @return the field
     */
    GField getField() const {return _field; }


    /**
     * @brief find the prime factors of this number
     * @param arraySize the array size pointer
     * @return an array of this numbers prime factors
     */
    GFNumber* getPrimeFactors(int *arraySize) const;


    /**
     * @brief prints this numbers prime factors
     */
    void printFactors();


    /**
     * @brief finds out if this number is a prime number
     * @return true if it is prime, false if not
     */
    bool getIsPrime() const {return GField::isPrime(_n); }


    /**
     * @brief operator overload
     */
    GFNumber& operator=(const GFNumber& other);


    /**
    * @brief operator overload
    */
    GFNumber& operator+=(const GFNumber& other);


    /**
     * @brief operator overload
     */
    GFNumber& operator+=(long other);


    /**
     * @brief operator overload
     */
    GFNumber operator+(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    GFNumber operator+(long other) const {return GFNumber(this->_n + other, this->_field); }


    /**
    * @brief operator overload
    */
    GFNumber& operator-=(const GFNumber& other);


    /**
     * @brief operator overload
     */
    GFNumber& operator-=(long other);


    /**
     * @brief operator overload
     */
    GFNumber operator-(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    GFNumber operator-(long other) const {return GFNumber(this->_n - other, this->_field); }


    /**
     * @brief operator overload
     */
    GFNumber& operator*=(const GFNumber& other);


    /**
    * @brief operator overload
    */
    GFNumber& operator*=(long other);


    /**
     * @brief operator overload
     */
    GFNumber operator*(const GFNumber& other) const;


    /**
    * @brief operator overload
    */
    GFNumber operator*(long other) const {return GFNumber(this->_n * other, this->_field); }


    /**
     * @brief operator overload
     */
    GFNumber& operator%=(const GFNumber& other);


    /**
     * @brief operator overload
     */
    GFNumber& operator%=(long other);


    /**
     * @brief operator overload
     */
    GFNumber operator%(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    GFNumber operator%(long other) const;


    /**
     * @brief operator overload
     */
    bool operator==(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    bool operator==(long other) const {return this->_n == other; }


    /**
     * @brief operator overload
     */
    bool operator!=(const GFNumber& other) const {return !(*this == other); }


    /**
     * @brief operator overload
     */
    bool operator!=(long other) const {return this->_n == other; }


    /**
     * @brief operator overload
     */
    bool operator<(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    bool operator<(long other) const {return this->_n < other; }


    /**
     * @brief operator overload
     */
    bool operator<=(const GFNumber& other) const;


    /**
     * @brief operator overload
     */
    bool operator<=(long other) const {return this->_n <= other; }


    /**
     * @brief operator overload
     */
    bool operator>=(const GFNumber& other) const {return !(*this < other); }


    /**
     * @brief operator overload
     */
    bool operator>=(long other) const {return !(*this < other); }


    /**
     * @brief operator overload
     */
    bool operator>(const GFNumber& other) const {return !(*this <= other); }


    /**
     * @brief operator overload
     */
    bool operator>(long other) const {return !(*this <= other); }


    /**
     * @brief operator overload
     */
    friend std::ostream& operator<<(std::ostream& stream, const GFNumber& number);


    /**
     * @brief operator overload
     */
    friend std::istream& operator>>(std::istream& stream, GFNumber& number);
};




#endif