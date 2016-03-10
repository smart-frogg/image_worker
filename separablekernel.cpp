#include "separablekernel.h"

SeparableKernel::SeparableKernel(int height,
        int width,
        unique_ptr<double[]> &fCol,
        unique_ptr<double[]> &fRow) : FilterKernel(height, width)
{
    this->fCol = move(fCol);
    this->fRow = move(fRow);
}

unique_ptr<ImageMap> SeparableKernel::apply(const ImageMap &data) const
{
    int imageHeight = data.getHeight();
    int imageWidth = data.getWidth();
    unique_ptr<ImageMap> tmp = make_unique<ImageMap>(imageHeight,imageWidth);
    unique_ptr<ImageMap> res = make_unique<ImageMap>(imageHeight,imageWidth);

    //auto column = make_unique<double[]>(imageHeight);
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double brightness = 0;
            for (int kx = 0; kx < width; kx++)
                brightness += fRow[width-1-kx] * data.getData(i+kx-width/2,j);
            tmp->setData(brightness,i,j);
        }
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double brightness = 0;
            for (int ky = 0; ky < height; ky++)
            {
                brightness += fCol[height-1-ky] * tmp->getData(i,j+ky-height/2);
            }
            res->setData(brightness,i,j);
        }
    return res;
}
