#include "piramid.h"
#include "gausskernelfactory.h"
#include <algorithm>
#include <iostream>
#include <math.h>

Piramid::Piramid(int countLayers, double sigma0, double sigmaFirst, const ImageMap &input)
{
    this->countLayers = countLayers;
    this->sigma0 = sigma0;
    this->sigmaFirst = sigmaFirst;
    height = input.getHeight();
    width = input.getWidth();
    calculateOctavs(input);
}

double Piramid::getHeight()
{
    return height;
}

double Piramid::getWidth()
{
    return width;
}

double Piramid::getSigmaFirst()
{
    return sigmaFirst;
}

double Piramid::getSigmaLast()
{
    return sigmaFirst * pow(2,octavs.size());
}

double Piramid::getSigmaStep()
{
    return pow(2,1./countLayers);
}
unique_ptr<QImage> Piramid::asImage()
{
    return octavs[0]->asImage();
}

void Piramid::calculateOctavs(const ImageMap &input)
{
    int size = min(input.getHeight(),input.getWidth());

    double sigma = sqrt(sigmaFirst*sigmaFirst - sigma0*sigma0);
    unique_ptr<FilterKernel> filter = GaussKernelFactory::getFilter(sigma);
    unique_ptr<ImageMap> curr = filter->apply(input);
    int i = 0;
    while (size > MIN_SIZE)
    {
        octavs.push_back(make_unique<Octave>(countLayers, sigmaFirst, i, *curr));
        const auto& octave = octavs.back();
        curr = octave->scale();
        size = min(curr->getHeight(),curr->getWidth());
        i++;
    }
}

void Piramid::save(QString filename)
{
    int i=0;
    for(const auto & octave : octavs)
    {
        octave->save(filename+"_"+QString::number(i));
        i++;
    }
}

void Piramid::saveDOG(QString filename)
{
    int i=0;
    for(const auto & octave : octavs)
    {
        octave->saveDOG(filename+"_"+QString::number(i));
        i++;
    }
}

void Piramid::genDescriptors()
{
    for(const auto & octave : octavs)
    {
        octave->genDescriptors(&descriptors);
    }
}

void Piramid::findGoodPoints()
{
   usingPoints = make_unique<bool[]>(descriptors.size());
   for (int i=0;i<descriptors.size();i++)
   {
      for (int j=i+1;j<descriptors.size();j++)
        if (descriptors[i].getPoint()->destination(*(descriptors[j].getPoint()))<16)
        {
            descriptors[i].getPoint()->count++;
            descriptors[j].used = false;
        }
   }
   for (int i=0;i<descriptors.size();i++)
   {
      if (descriptors[i].getPoint()->count>5 && usingPoints[i])
      {
          double max = pow(2,octavs.size());
          Point *p = descriptors[i].getPoint();
          double step = max/octavs[0]->size();
          double maxVal = L(p->x,p->y,p->sigma);
          double minVal = L(p->x,p->y,p->sigma);
          double maxSigma = 0;
          double minSigma = 0;

          for (double sigma=1; sigma < max; sigma+=step)
          {
            for (int i=p->x-4;i<p->x+4;i++)
              for (int j=p->y-4;j<p->y+4;j++)
              {
                  double v = L(i,j,sigma+step)*sigma-L(i,j,sigma)*sigma;
                  if (v>maxVal)
                  {
                      maxSigma = sigma;
                      maxVal = v;
                  }
                  if (v<maxVal)
                  {
                      minSigma = sigma;
                      minVal = v;
                  }
              }
          }
          p->sigma = fabs(maxVal)>fabs(minVal)?maxSigma:minSigma;
      }
   }
}

void Piramid::saveDescriptors(QString filename)
{
    unique_ptr<QImage> image = this->octavs[0]->layers[0]->asImage();
    unique_ptr<QPainter> painter = make_unique<QPainter>(image.get());
   // painter->setBrush(Qt::green);

    for (int i=0;i<descriptors.size();i++) if (descriptors[i].used)
    {

        painter->setPen(qRgb(rand()%255,rand()%255,rand()%255));
        Point *p1 = descriptors[i].getPoint();
        double r = p1->sigma*sqrt(2)+1;
        painter->drawEllipse(p1->x*p1->scale-r,p1->y*p1->scale-r,2*r,2*r);
    }
    image->save(filename+"descriptors.jpg","JPG", 100);
}

double Piramid::L(int x,int y,double sigma)
{
    int k = ceil(log2(sigma/sigmaFirst)) - 1;
    if (k < 0) k = 0;
    if (k >= octavs.size()) k = octavs.size()-1;
   // cout << "octave is " << k <<endl;
    int p = 1<<k;
    return octavs[k]->L(x/p, y/p, sigma/p);
}
