#ifndef SIMPLEKERNEL_H
#define SIMPLEKERNEL_H

#include "filterkernel.h"
class SimpleKernel : public FilterKernel
{
private:
    unique_ptr<double[]> kernel;
public:
    unique_ptr<ImageMap> apply(const ImageMap &data);
    SimpleKernel(int height, int width, unique_ptr<double[]> &kernel);
};

#endif // SIMPLEKERNEL_H
