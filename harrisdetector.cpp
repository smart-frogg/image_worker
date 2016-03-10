#include "harrisdetector.h"

HarrisDetector::HarrisDetector(ImageMap *data):AbstractDetector(data)
{
    sobelFilter = make_unique<SobelFilter>();
    sobelFilter->apply(*data);
}

void HarrisDetector::detect(double T, int winR)
{
    int imageHeight = data->getHeight();
    int imageWidth = data->getWidth();
    ImageMap *dX = sobelFilter->getImageX();
    ImageMap *dY = sobelFilter->getImageY();
    unique_ptr<ImageMap> f = make_unique<ImageMap>(imageHeight,imageWidth);

    //auto column = make_unique<double[]>(imageHeight);
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double cur_dX = dX->getData(i,j);
            double cur_dY = dY->getData(i,j);
            double A = cur_dX*cur_dX;
            double B = cur_dX*cur_dY;
            double C = cur_dY*cur_dY;
            double det = A*B - C*C;
            double trace = A + B;
            f->setData(i, j, det - trace * trace);
        }
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double curF = f->getData(i,j);
            if (curF < T) continue;
            bool isMax = true;
            for (int k1 = -winR; k1<=winR; k1++)
                for (int k2 = -winR; k2<=winR; k2++)
                {
                    isMax = isMax && (curF > f->getData(k1,k2));
                }
            if (isMax)
            {
                points.push_back(Point(i,j));
            }
        }
}
