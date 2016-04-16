#include "blobdetector.h"
#include <math.h>
#include <iostream>

BlobDetector::BlobDetector(Piramid *p)
{
    data = p;
}

Blob::Blob(int x, int y, double sigma)
{
    this->x = x;
    this->y = y;
    this->sigma = sigma;
    this->r = sigma*sqrt(2);
    cout<<this->r<<endl;
}

void BlobDetector::save(QString filename)
{
    unique_ptr<QImage> image = this->data->asImage();
    unique_ptr<QPainter> painter = make_unique<QPainter>(image.get());

    painter->setPen(qRgb(0,100,0));
    for(Blob &b:blobs) if (b.used)
    {
        painter->drawEllipse(b.x-1, b.y-1,2,2);
        painter->drawEllipse(b.x-b.r, b.y-b.r,2*b.r,2*b.r);
    }
    image->save(filename+"_blobs.jpg","JPG", 100);
}

void BlobDetector::detect()
{
    double sigmaFirst = data->getSigmaFirst();
    double sigmaStep = data->getSigmaStep();
    for (int oID = 0; oID < data->octavs.size(); oID++)
    {
        data->octavs[oID]->genDOG();
        double sigma = sigmaFirst*(1<<oID);

        for (int layerID = 1; layerID < data->octavs[oID]->dogs.size()-1; layerID++)
        {
            ImageMap* img =  data->octavs[oID]->dogs[layerID].get();
            for (int x=0;x<img->getWidth();x++)
            {
                for (int y=0;y<img->getHeight();y++)
                {
                    bool isMax = true;
                    bool isMin = true;
                    double val = img->getData(x,y);//* sigma;
                    //if(val<0.01) continue;
                    //double ds =  sigma/sigmaStep;
                    for (int dl = layerID-1; dl <= layerID+1; dl++)
                    {
                        ImageMap* img1 =  data->octavs[oID]->dogs[dl].get();
                        for(int dx = x-1; dx <= x+1; dx++)
                        {
                            for(int dy = y-1; dy <= y+1; dy++)
                            {
                                double val1 = img1->getData(dx,dy);//*ds;
                                if (dx != x || dy != y || dl != layerID)
                                {
                                    isMax &= (val1 < val);
                                    isMin &= (val1 > val);
                                }

                            }
                        }
                      //  ds*=sigmaStep;
                    }
                    if (isMax || isMin)
                    {
                        blobs.push_back(Blob(x*(oID+1),y*(oID+1),sigma));
                    }
               }
          }
            sigma*=sigmaStep;
       }
    }
}
