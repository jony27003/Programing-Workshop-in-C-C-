#include <cassert>
#include "GField.h"
#include "GFNumber.h"
using std::cout;
using std::endl;




/**
 * @brief a constructor
 * this can be used as a default constructor or as a non default constructor.
 * @param n a given number
 * @param field a given field
 */
GFNumber::GFNumber(const long n, const GField& field)
{
    _field = field;
    _n = ((n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
}


/**
 * @brief find the prime factors of this number
 * @param arraySize the array size pointer
 * @return an array of this numbers prime factors
 */
GFNumber* GFNumber::getPrimeFactors(int *arraySize) const
{
    *arraySize = 0;
    long num = this->getNumber();
    if (getIsPrime() || num == -1 || num == 0 || num == 1)
    {
        return new GFNumber[0];
    }
    GFNumber *factors = nullptr;
    for(int j = 2; j <= 3; ++j)
    {
        int counter = 0;
        while(num % j == 0)
        {
            ++counter;
            num /= j;
        }
        if(counter > 0)
        {
            factors = _correctArray(factors, *arraySize, *arraySize + counter);
            for(long i = *arraySize - counter; i < *arraySize; ++i)
            {
                factors[i] = GFNumber(j, this->getField());
            }
        }
    }
    if (GField::isPrime(num))
    {
        factors = _correctArray(factors, *arraySize, *arraySize + 1);
        factors[*arraySize - 1] = GFNumber(num, this->getField());
        return factors;
    }
    long prime = 0;
    while (num != 1)
    {
        prime = _longFactorization(num);
        if (prime != -1)
        {
            factors = _correctArray(factors, *arraySize, *arraySize + 1);
            factors[*arraySize - 1] = GFNumber(prime, this->getField());
            num /= prime;
        }
        else
        {
            break;
        }
    }
    long i = 2;
    while (i * i <= num)
    {
        if (num % i == 0)
        {
            factors = _correctArray(factors, *arraySize, *arraySize + 1);
            factors[*arraySize - 1] = GFNumber(i, this->getField());
            num /= i;
        }
        else
        {
            ++i;
        }
    }
    if (num > 1)
    {
        factors = _correctArray(factors, *arraySize, *arraySize + 1);
        factors[*arraySize - 1] = GFNumber(num, this->getField());
    }
    return factors;
}


/**
 * @brief prints this numbers prime factors
 */
void GFNumber::printFactors()
{
    int arraySize;
    GFNumber *factors = getPrimeFactors(&arraySize);
    cout << _n << "=";
    if (arraySize == 0)
    {
        cout << _n << "*1" << endl;
        delete[] factors;
        return;
    }
    cout << factors[0].getNumber();
    for (int i = 1; i < arraySize; ++i)
    {
        if(factors[i].getNumber() != 1)
        {
            cout << "*" << factors[i].getNumber();
        }
    }
    cout << endl;
    delete[] factors;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator=(const GFNumber& other)
{
    this->_field = other._field;
    this->_n = other._n;
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator+=(const GFNumber& other)
{
    assert(this->_field == other._field);
    this->_n += other._n;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator+=(long other)
{
    this->_n += other;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber GFNumber::operator+(const GFNumber &other) const
{
    assert(this->_field == other._field);
    return GFNumber(this->_n + other._n, this->_field);
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator-=(const GFNumber& other)
{
    assert(this->_field == other._field);
    this->_n -= other._n;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator-=(long other)
{
    this->_n -= other;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber GFNumber::operator-(const GFNumber &other) const
{
    assert(this->_field == other._field);
    return GFNumber(this->_n - other._n, this->_field);
}



/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator*=(const GFNumber &other)
{
    assert(this->_field == other._field);
    this->_n *= other._n;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator*=(long other)
{
    this->_n *= other;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber GFNumber::operator*(const GFNumber &other) const
{
    assert(this->_field == other._field);
    return GFNumber(this->_n * other._n, this->_field);
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator%=(const GFNumber &other)
{
    assert(this->_field == other._field);
    assert(other.getNumber() != 0);
    *this = *this % other;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber& GFNumber::operator%=(long other)
{
    assert(other != 0);
    GFNumber num = GFNumber(other, this->_field);
    *this = *this % num;
    _n = ((_n % _field.getOrder()) + _field.getOrder()) % _field.getOrder();
    return *this;
}


/**
 * @brief operator overload
 */
GFNumber GFNumber::operator%(const GFNumber &other) const
{
    assert(this->_field == other._field);
    assert(other.getNumber() != 0);
    return GFNumber(this->_n % other._n, this->_field);
}


/**
 * @brief operator overload
 */
GFNumber GFNumber::operator%(long other) const
{
    assert(other != 0);
    return GFNumber(this->_n % other, this->_field);
}


/**
 * @brief operator overload
 */
bool GFNumber::operator==(const GFNumber &other) const
{
    bool flag1 = this->_n == other._n;
    bool flag2 = this->_field.getChar() == other._field.getChar();
    return (flag1 && flag2);
}


/**
 * @brief operator overload
 */
bool GFNumber::operator<(const GFNumber &other) const
{
    assert(this->_field == other._field);
    return this->_n < other._n;
}


/**
 * @brief operator overload
 */
bool GFNumber::operator<=(const GFNumber &other) const
{
    assert(this->_field == other._field);
    return this->_n <= other._n;
}


/**
 * @brief operator overload
 */
std::ostream& operator<<(std::ostream& stream, const GFNumber& number)
{
    stream << number.getNumber() << " " << number.getField();
    assert(!stream.fail());
    return stream;
}


/**
 * @brief operator overload
 */
std::istream& operator>>(std::istream &stream, GFNumber &number)
{
    stream >> number._n >> number._field;
    assert(!stream.fail());
    long n = number._n;
    long order = number._field.getOrder();
    number._n = ((n % order) + order) % order;
    return stream;
}


/** the factorization algorithm */
long GFNumber::_longFactorization(long num) const
{
    long maxRandom = num - 1;
    long randomNumber = rand() % maxRandom + 1;
    GFNumber x = GFNumber(randomNumber, this->_field);
    GFNumber y = x;
    GFNumber p(1, this->_field);
    long temp;
    while (p.getNumber() == (long)1)
    {
        x = GFNumber(_function(x.getNumber()), this->_field);
        y = GFNumber(_function(_function(x.getNumber())), this->_field);

        temp = x.getNumber() - y.getNumber();
        if(temp < 0)
        {
            temp = -temp;
        }
        p = _field.gcd(GFNumber(temp, p.getField()), GFNumber(num, this->getField()));
    }
    if (p.getNumber() == num)
    {
        return -1;
    }
    return p.getNumber();
}


/** a method to copy an array and update its size */
GFNumber* GFNumber::_correctArray(GFNumber *array, int &arraySize, int newArraySize)
{
    GFNumber *newArray = new GFNumber[newArraySize];
    if (array == nullptr)
    {
        arraySize = newArraySize;
        return newArray;
    }
    int i = 0;
    while (i < arraySize)
    {
        newArray[i] = array[i];
        ++i;
    }
    arraySize = newArraySize;
    delete[] array;
    return newArray;
}