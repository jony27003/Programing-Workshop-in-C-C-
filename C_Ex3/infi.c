#include "infi.h"
#include <assert.h>

/**
 * @brief
 */
double integration(RealFunction func, double a, double b, unsigned int n)
{
    assert(b >= a);
    double dx = (b - a) / n;
    double sum = 0;
    double mid;

    int i = 0;
    while(i < n)
    {
        mid = a + (i * dx) + (dx / 2);
        sum = sum + func(mid);
        ++i;
    }
    return (sum * dx);
}

/**
 * @brief
 */
double derivative(RealFunction func, double x0, double h)
{
    assert(h > 0);
    return ((func(x0 + h) - func(x0 - h)) / (2 * h));
}
