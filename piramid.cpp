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
    double sigma = sigmaFirst;
    for(const auto & octave : octavs)
    {
        octave->genDescriptors(&descriptors,sigma);
        sigma*=2;
    }
}
void  Piramid::genDOG()
{
    for (int oID = 0; oID < octavs.size(); oID++)
    {
        octavs[oID]->genDOG();
    }
}
void Piramid::genBlobs()
{
    blobs.clear();
    unique_ptr<BlobDetector> detector = make_unique<BlobDetector>(this);
    detector->detect();
    detector->save(QString(BASE_PATH));
}

void Piramid::findGoodPoints()
{
   //usingPoints = make_unique<bool[]>(descriptors.size());
    double sigmaStep = getSigmaStep();
    for (int i=0;i<descriptors.size();i++)
    {
        Point *p1 = descriptors[i].getPoint();
        p1->laplasian=L(p1->x,p1->y,p1->sigma)-L(p1->x,p1->y,p1->sigma/sigmaStep);
        descriptors[i].used = true;
    }
   for (int i=0;i<descriptors.size();i++) if (descriptors[i].used )
   {
      int goodPointMin = i;
      int goodPointMax = i;
      double min = descriptors[i].getPoint()->laplasian;
      double max = descriptors[i].getPoint()->laplasian;
      for (int j=i+1;j<descriptors.size();j++)
        if (descriptors[j].used &&
           descriptors[i].getPoint()->destination(*(descriptors[j].getPoint()))<(1<<abs(descriptors[i].getPoint()->scale-descriptors[j].getPoint()->scale))*4)
        {
            if(descriptors[j].getPoint()->laplasian > max)
            {
                max = descriptors[j].getPoint()->laplasian;
                goodPointMax = j;
            }
            if(descriptors[j].getPoint()->laplasian < min)
            {
                min = descriptors[j].getPoint()->laplasian;
                goodPointMin = j;
            }
            descriptors[j].used = false;
        }
      if (max>0.03)
      {
          descriptors[goodPointMax].used = true;
          cout<<max<<endl;
      }
      if (min<-0.03)
      {
          descriptors[goodPointMin].used = true;
            cout<<min<<endl;
      }
   }
   /*for (int i=0;i<descriptors.size();i++)
   {
      Point *p = descriptors[i].getPoint();
      if (p->count>5 && descriptors[i].used)
      {
          Blob *b;
          double min = sqrt((height-width)*(height-width))+5;
          for(int i=0;i<blobs.size(); i++) {
            Blob blob = blobs[i];
            double dest = p->destination(blob);
            if (dest<p->count*8)
            {
                min = dest;
                b = &blob;
            }
          }
          if (min<16)
          {
              p->setSigma(b->sigma);
              p->b = b;

          }
      }
   }*/
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
        double scale = 1<<p1->scale;
        painter->drawEllipse(p1->x*scale-r,p1->y*scale-r,2*r,2*r);
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

vector<Descriptor> *Piramid::getDescriptors()
{
    return &descriptors;
}
