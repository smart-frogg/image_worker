#include "moravekdetector.h"
#include "sobelfilter.h"
#include <iostream>

MoravekDetector::MoravekDetector(ImageMap *data):AbstractDetector(data)
{
    d[0][0] = -1;
    d[0][1] = -1;
    d[1][0] = -1;
    d[1][1] = 0;
    d[2][0] = -1;
    d[2][1] = 1;
    d[3][0] = -1;
    d[3][1] = 0;
    d[4][0] = 0;
    d[4][1] = 1;
    d[5][0] = 1;
    d[5][1] = -1;
    d[6][0] = 1;
    d[6][1] = 0;
    d[7][0] = 1;
    d[7][1] = 1;
/*    d[0][0] = -1;
    d[0][1] = -1;
    d[1][0] = -1;
    d[1][1] = 0;
    d[2][0] = -1;
    d[2][1] = 1; */

}

void MoravekDetector::configure(double T, int winR)
{
    this->T = T;
    this->winR = winR;
}
void MoravekDetector::detect()
{
    int imageHeight = data->getHeight();
    int imageWidth = data->getWidth();
    unique_ptr<ImageMap> f = make_unique<ImageMap>(imageHeight,imageWidth);

    //auto column = make_unique<double[]>(imageHeight);
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
        {
            double minC = 100000;
            for(int di=0; di<8; di++)
            {
                double C = 0;
                for (int k1 = -winR; k1<=winR; k1++)
                    for (int k2 = -winR; k2<=winR; k2++)
                    {
                        double tmp = data->getData(i+k1,j+k2) - data->getData(i+k1+d[di][0],j+k2+d[di][1]);
                        C += tmp*tmp;
                    }
                if (C < minC)
                    minC = C;
            }
            f->setData(minC, i, j);
        }
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
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
