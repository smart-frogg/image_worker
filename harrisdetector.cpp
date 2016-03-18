#include "harrisdetector.h"
#include "gausskernelfactory.h"
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

    unique_ptr<ImageMap> A = make_unique<ImageMap>(imageHeight,imageWidth);
    unique_ptr<ImageMap> B = make_unique<ImageMap>(imageHeight,imageWidth);
    unique_ptr<ImageMap> C = make_unique<ImageMap>(imageHeight,imageWidth);
    //auto column = make_unique<double[]>(imageHeight);
    for (unsigned int i = 0; i < imageWidth; i++)
        for (unsigned int j = 0; j < imageHeight; j++)
        {
            double cur_dX = dX->getData(i,j);
            double cur_dY = dY->getData(i,j);
            A->setData(cur_dX*cur_dX,i,j);
            B->setData(cur_dX*cur_dY,i,j);
            C->setData(cur_dY*cur_dY,i,j);
        }
    unique_ptr<SeparableKernel> filter = GaussKernelFactory::getFilter(5);
    A = filter->apply(*A);
    B = filter->apply(*B);
    C = filter->apply(*C);
    double avg = 0;
    for (unsigned int i = 0; i < imageWidth; i++)
        for (unsigned int j = 0; j < imageHeight; j++)
        {
            double cur_A = A->getData(i,j);
            double cur_B = B->getData(i,j);
            double cur_C = C->getData(i,j);
            double det = cur_A*cur_C - cur_B*cur_B;
            double trace = cur_A + cur_C;
            f->setData(det - k * trace * trace, i, j);
            avg+=det - k * trace * trace;
        }
    cout<<(avg/(imageWidth*imageHeight))<<endl;
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
                points.emplace_back(i,j);
            }
        }
}
