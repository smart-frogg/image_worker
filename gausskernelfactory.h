#ifndef GAUSSKERNELFACTORY_H
#define GAUSSKERNELFACTORY_H

#include "separablekernel.h"

class GaussKernelFactory
{
public:
    static unique_ptr<SeparableKernel> getFilter(double sigma);
    static unique_ptr<SeparableKernel> gaussian;
    static void init();
    GaussKernelFactory();
};

#endif // GAUSSKERNELFACTORY_H
