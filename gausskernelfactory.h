#ifndef GAUSSKERNELFACTORY_H
#define GAUSSKERNELFACTORY_H

#include "separablekernel.h"

class GaussKernelFactory
{
public:
    GaussKernelFactory();
    static unique_ptr<SeparableKernel> getFilter(double sigma);
};

#endif // GAUSSKERNELFACTORY_H
