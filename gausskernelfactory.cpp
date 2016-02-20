#include "gausskernelfactory.h"
#define _USE_MATH_DEFINES
#include <math.h>

GaussKernelFactory::GaussKernelFactory()
{

}

unique_ptr<SeparableKernel> GaussKernelFactory::getFilter(double sigma)
{
    int size = sigma*3;
    unique_ptr<double[]> fCol = make_unique<double[]>(2*size+1);
    unique_ptr<double[]> fRow = make_unique<double[]>(2*size+1);
    double d = sigma*sqrt(2*M_PI);
    for (int i=-size; i<=size; i++)
    {
        fCol[i+size] = exp(-i*i/(2*sigma*sigma))/d;
    }
    for (int i=0; i<2*size+1; i++)
    {
        //fCol[i] /= sum;
        fRow[i] = fCol[i];
    }
    return make_unique<SeparableKernel>(2*size+1,2*size+1,fCol,fRow);
}
