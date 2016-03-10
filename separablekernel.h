#ifndef SEPARABLEKERNEL_H
#define SEPARABLEKERNEL_H
#include "filterkernel.h"

class SeparableKernel : public FilterKernel
{
private:
    unique_ptr<double[]> fCol;
    unique_ptr<double[]> fRow;
public:
    SeparableKernel(int height, int width, unique_ptr<double[]> &fCol, unique_ptr<double[]> &fRow) ;
    unique_ptr<ImageMap> apply(const ImageMap &data) ;
};

#endif // SEPARABLEKERNEL_H
