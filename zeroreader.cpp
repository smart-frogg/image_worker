#include "zeroreader.h"

ZeroReader::ZeroReader()
{

}

double ZeroReader::get(int x, int y)
{
    if (image == nullptr)
        return 0;
    if ((*image)->getHeight() <= y || 0 > y ||
        (*image)->getWidth() <= x || 0 > x)
        return 0;
    return (*image)->getData(x,y);
}
unique_ptr<ImageReader> ZeroReader::copy()
{
    return make_unique<ZeroReader>();
}
