#include "descriptor.h"
#include "gausskernelfactory.h"
#include <iostream>
Histogram::Histogram()
{
    size = BUSKETS_COUNT;
    clearBuskets();
}

Histogram::Histogram(int size)
{
    this->size = size;
    clearBuskets();
}

void Histogram::clearBuskets()
{
    buskets = make_unique<double[]>(size);
    for(int i=0;i<size;i++)
        buskets[i]=0;
}

Descriptor::Descriptor(const Point &point, SobelFilter *sobelFilter)
{
    this->point = Point(point);
    this->sobelFilter = sobelFilter;
    clothest=NULL;
    for(int i=-4;i<4;i++)
        for(int j=-4;j<4;j++)
        {
            double phi = point.angle(point.x+i,point.y+j);
            double m = sobelFilter->getM(point.x+i,point.y+j);
            GaussKernelFactory::gaussian->apply(m,i,j);
            int num = phi/(M_PI/2);
            hists[num].put(m,sobelFilter->getAlpha(point.x+i,point.y+j)-point.orientation);
        }

     for (int i=0;i<4;i++)
     {
         hists[i].normalize();
         hists[i].crop();
         hists[i].normalize();
     }
}

double Descriptor::destination(const Descriptor &descriptor) const
{
    double sum = 0;
    for(int i=0;i<4;i++)
    {
        sum += hists[i].destination(descriptor.hists[i]);
    }
    //cout<<sum<<endl;
    return sum;
}

double Histogram::destination(const Histogram &hist) const
{
    double sum = 0;
    for(int i=0;i<size;i++)
    {
        sum += (buskets[i]-hist.buskets[i])*(buskets[i]-hist.buskets[i]);
    }
    return sum;
}

void Descriptor::findClothest(vector<Descriptor> *descriptors)
{
    double minDestination = -1;
    clothest = NULL;
    for (int i=0,n=descriptors->size();i<n;i++)
    {
        double dest = destination((*descriptors)[i]);
        if (dest < minDestination || minDestination < 0)
        {
            minDestination = dest;
            clothest = &((*descriptors)[i]);
        }
    }
    //cout<<minDestination<<endl;
}

Point* Descriptor::getPoint()
{
    return &point;
}

Descriptor* Descriptor::getClothest()
{
    return clothest;
}

void Histogram::calcMax(double t)
{
    for (int i=0;i<size;i++)
    {
        if (firstMax == -1)
        {
            firstMax = i;
            continue;
        }
        if (buskets[i]>buskets[firstMax])
        {
            if (buskets[firstMax]>t)
                secondMax = firstMax;
            firstMax = i;
            continue;
        }
        if (buskets[i]<t) continue;
        if (secondMax == -1)
        {
            secondMax = i;
            continue;
        }
        if (buskets[i]>buskets[secondMax])
        {
            secondMax = i;
            continue;
        }
    }
}

double Histogram::getFirstMax()
{
    return 2*M_PI*firstMax/size;
}

double Histogram::getSecondMax()
{
    return 2*M_PI*secondMax/size;
}

void Histogram::normalize()
{
    auto result = minmax_element(buskets.get(),buskets.get()+size);
    double floor = *(result.second)-*(result.first);
    if (floor<0.0000001)
        return;
    for (int i=0;i<size;i++)
    {
        buskets[i] = (buskets[i]-*(result.first))/floor;
    }
}
void Histogram::crop()
{
    for (int i=0;i<size;i++)
    {
        if (buskets[i]>0.2) buskets[i] = 0.2;
    }
}

void Histogram::put(double m, double phi)
{
    while (phi >= 2*M_PI-0.0000001)
    {
      //  cout <<" "<<phi<<" reduce " <<endl;
        phi-=2*M_PI;
        if (phi<0)
            phi = 0;
    }
    while (phi < 0)
    {
      //  cout <<" "<<phi<<" up " <<endl;
        phi+=2*M_PI;
    }
    double fragment = 2*M_PI/size;
    int iMin = phi/fragment;
    double r1 = phi - iMin*fragment;
    double r2 = (1+iMin)*fragment - phi;
   // if (iMin >= size || iMin < 0)
   //     cout <<iMin<<" "<<phi<<" "<<size <<endl;
    buskets[iMin%size] += r2*m/fragment;
    buskets[(iMin+1)%size] += r1*m/fragment;
}

