#include "sobelfilter.h"

SobelFilter::SobelFilter() : FilterKernel(3, 3)
{
    unique_ptr<double[]> fRowY = make_unique<double[]>(3);
    fRowY[0] = fRowY[2] = 1;
    fRowY[1] = 2;
    unique_ptr<double[]> fColY = make_unique<double[]>(3);
    fColY[0] = -1;
    fColY[1] = 0;
    fColY[2] = 1;
    unique_ptr<double[]> fRowX = make_unique<double[]>(3);
    fRowX[0] = -1;
    fRowX[1] = 0;
    fRowX[2] = 1;
    unique_ptr<double[]> fColX = make_unique<double[]>(3);
    fColX[0] = fColX[2] = 1;
    fColX[1] = 2;

    gy = make_unique<SeparableKernel>(3,3,fColY,fRowY);
    gx = make_unique<SeparableKernel>(3,3,fColX,fRowX);
}

unique_ptr<ImageMap> SobelFilter::apply(const ImageMap &data)
{
    imageX = gx->apply(data);
    imageY = gy->apply(data);
    int imageHeight = data.getHeight();
    int imageWidth = data.getWidth();
    unique_ptr<ImageMap> res = make_unique<ImageMap>(imageHeight,imageWidth);

    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double x = imageX->getData(i,j);
            double y = imageY->getData(i,j);

            double brightness = sqrt(x*x+y*y);
            res->setData(brightness,i,j);
        }
    res->normalize();
    return res;
}
ImageMap * SobelFilter::getImageX()
{
    return imageX.get();
}

ImageMap *SobelFilter::getImageY()
{
    return imageY.get();
}

