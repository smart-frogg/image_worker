#include "simplekernel.h"

SimpleKernel::SimpleKernel(int height,
        int width,
        unique_ptr<double[]> &kernel) : FilterKernel(height, width)
{
    this->kernel = move(kernel);
}

unique_ptr<ImageMap> SimpleKernel::apply(const ImageMap &data)
{
    int imageHeight = data.getHeight();
    int imageWidth = data.getWidth();
    unique_ptr<ImageMap> res = make_unique<ImageMap>(imageHeight,imageWidth);

    //auto column = make_unique<double[]>(imageHeight);
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double brightness = 0;
            for (int kx = 0; kx < width; kx++)
                for (int ky = 0; ky < height; ky++)
                brightness += this->kernel[((height-1-ky)*width +(width-1-kx))] * data.getData(i+kx-width/2,j+ky-height/2);
            res->setData(brightness,i,j);
        }
    return res;
}
