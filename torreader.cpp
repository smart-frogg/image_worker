#include "torreader.h"

TorReader::TorReader()
{

}

double TorReader::get(int x, int y)
{
    if (image == nullptr)
        return 0;
    return (*image)->getData(((*image)->getWidth() + x) % (*image)->getWidth(),
                         ((*image)->getHeight() + y) % (*image)->getHeight());
}
unique_ptr<ImageReader> TorReader::copy()
{
    return make_unique<TorReader>();
}
