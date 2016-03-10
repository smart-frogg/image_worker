#include "harrisdetector.h"
#include <iostream>

HarrisDetector::HarrisDetector(ImageMap *data):AbstractDetector(data)
{
    sobelFilter = make_unique<SobelFilter>();
    sobelFilter->apply(*data);
}
void HarrisDetector::configure(double T, double k, int winR)
{
    this->T = T;
    this->k = k;
    this->winR = winR;
}

void HarrisDetector::detect()
{
    int imageHeight = data->getHeight();
    int imageWidth = data->getWidth();
    ImageMap *dX = sobelFilter->getImageX();
    ImageMap *dY = sobelFilter->getImageY();
    unique_ptr<ImageMap> f = make_unique<ImageMap>(imageHeight,imageWidth);

    //auto column = make_unique<double[]>(imageHeight);
    for (unsigned int i = 0; i < imageWidth; i++)
        for (unsigned int j = 0; j < imageHeight; j++)
        {
            double cur_dX = dX->getData(i,j);
            double cur_dY = dY->getData(i,j);
            double A = cur_dX*cur_dX;
            double B = cur_dX*cur_dY;
            double C = cur_dY*cur_dY;
            double det = A*B - C*C;
            double trace = A + B;
            f->setData(det - k * trace * trace, i, j);
        }
    for (unsigned int i = 0; i < imageWidth; i++)
        for (unsigned int j = 0; j < imageHeight; j++)
        {
            double curF = f->getData(i,j);
            if (curF < T) continue;
            bool isMax = true;
            for (int k1 = -winR; k1<=winR; k1++)
                for (int k2 = -winR; k2<=winR; k2++) if (k1!=0 || k2!=0)
                {
                    isMax = isMax && (curF > f->getData(i+k1,j+k2));
                }
            if (isMax)
            {
                points.push_back(Point(i,j));
            }
        }
}
