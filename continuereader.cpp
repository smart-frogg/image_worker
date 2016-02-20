#include "continuereader.h"

ContinueReader::ContinueReader()
{

}
double ContinueReader::get(int x, int y)
{
    if (image == nullptr)
        return 0;
    int clearY = y;
    int clearX = x;

    if ((*image)->getHeight() <= clearY)
        clearY = (*image)->getHeight() - 1;

    if (0 > clearY)
        clearY = 0;

    if ((*image)->getWidth() <= clearX)
        clearX = (*image)->getWidth() - 1;

    if (0 > clearX)
        clearX = 0;

    return (*image)->getData(clearX,clearY);
}
unique_ptr<ImageReader> ContinueReader::copy()
{
    return make_unique<ContinueReader>();
}
