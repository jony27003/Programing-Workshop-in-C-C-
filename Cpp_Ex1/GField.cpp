#include <cmath>
#include <cassert>
#include "GField.h"
#include "GFNumber.h"




/**
 * @brief a constructor
 * this can be used as a default constructor or as a non default constructor.
 * @param p a given p
 * @param l a given l
 */
GField::GField(long p, long l)
{
    if (p < 0)
    {
        p = -p;
    }
    assert((p != 0) && (p != 1) && (l > 0) && isPrime(p));
    this->_p = p;
    this->_l = l;
}


/**
 * @brief checks is the given number is a prime number
 * @param number the number to check
 * @return true if prime, false if not
 */
bool GField::isPrime(long number)
{
    if (number < 0)
    {
        number = -number;
    }
    if (number == 1 || number == 0)
    {
        return true;
    }
    int i = 2;
    while(pow(i, 2) <= number)
    {
        if ((number % i) == 0)
        {
            return false;
        }
        ++i;
    }
    return true;
}


/**
 * @brief finds the gcd of the 2 numbers
 * @param a first number
 * @param a second number
 * @return the gcd
 */
GFNumber GField::gcd(const GFNumber &a, const GFNumber &b) const
{
    assert(a.getField() == b.getField());
    assert(*this == a.getField());
    if(a == 0 && b == 0)
    {
        // NO GCD.
        assert(0);
    }
    if (a == (long)0)
    {
        return b;
    }
    if (b == (long)0)
    {
        return a;
    }
    return gcd(b % a, a);
}


/**
 * @brief creats the number k in this field
 * @param k the number to create
 * @return a GNumber that represents k
 */
GFNumber GField::createNumber(long k)
{
    return GFNumber(k, *this);
}


/**
 * @brief operator overload
 */
GField& GField::operator=(const GField& other)
{
    this->_l = other._l;
    this->_p = other._p;
    return *this;
}


/**
 * @brief operator overload
 */
bool GField::operator==(const GField& other) const
{
    return (this->_p == other._p) && (this->_l == other._l);
}


/**
 * @brief operator overload
 */
std::istream& operator>>(std::istream& stream, GField& field)
{
    long p = (long)0;
    long l = (long)0;
    stream >> p >> l;
    assert(!stream.fail());
    field = GField(p, l);
    return stream;
}


/**
 * @brief operator overload
 */
std::ostream& operator<<(std::ostream& stream, const GField& field)
{
    long pOutput = field.getChar();
    long lOutput = field.getDegree();
    stream << "GF(" << pOutput << "**" << lOutput << ")";
    assert(!stream.fail());
    return stream;
}