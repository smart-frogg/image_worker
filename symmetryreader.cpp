#include "symmetryreader.h"

SymmetryReader::SymmetryReader()
{

}
double SymmetryReader::get(int x, int y)
{
    if (image == nullptr)
        return 0;
    int clearY = y;
    int clearX = x;

    if ((*image)->getHeight() <= clearY)
        clearY = 2 * (*image)->getHeight() - clearY;

    if (0 > clearY)
        clearY = - clearY;

    if ((*image)->getWidth() <= clearX)
        clearX = 2* (*image)->getWidth() - clearX;

    if (0 > clearX)
        clearX = - clearX;

    return (*image)->getData(clearX,clearY);
}
unique_ptr<ImageReader> SymmetryReader::copy()
{
    return make_unique<SymmetryReader>();
}
