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

Descriptor::Descriptor(const Point &point, int winSize, SobelFilter *sobelFilter)
{
    this->point = Point(point);
    this->sobelFilter = sobelFilter;
    clothest=NULL;
    for(int i=-winSize;i<winSize;i++)
        for(int j=-winSize;j<winSize;j++)
        {
            double phi = point.angle(point.x+i,point.y+j);
            double dest = sqrt(i*i+j*j);

            double m = sobelFilter->getM(point.x+i,point.y+j);
            m = GaussKernelFactory::gaussian->apply(m,i,j);
            double alpha = sobelFilter->getAlpha(point.x+i,point.y+j)-point.orientation;
            double x = dest*cos(phi)/winSize + HISTS_COUNT/2;
            double y = dest*sin(phi)/winSize + HISTS_COUNT/2;

            // Трилинейная интерполяция
            int I = x/HISTS_COUNT;
            int J = y/HISTS_COUNT;
            double dx = x-I;
            double dy = y-J;
            double coeffX[3];
            double coeffY[3];
            coeffX[0] = 0;
            coeffX[1] = 1;
            coeffX[2] = 0;
            coeffY[0] = 0;
            coeffY[1] = 1;
            coeffY[2] = 0;
            if (dx<=0.5 && I>0)
            {
                coeffX[0] = m*(0.5-dx);
                coeffX[1] = 0.5+dx;
            }
            else if (dx>0.5 && I<HISTS_COUNT-1)
            {
                coeffX[1] = 1.5-dx;
                coeffX[2] = dx-0.5;
            }
            if (dy<=0.5 && J>0)
            {
                coeffY[0] = m*(0.5-dy);
                coeffY[1] = 0.5+dy;
            }
            else if (dy>0.5 && J<HISTS_COUNT-1)
            {
                coeffY[1] = 1.5-dy;
                coeffY[2] = dy-0.5;
            }
            for (int hx=0;hx<3;hx++)
                for (int hy=0;hy<3;hy++)
                    if(coeffX[hx]*coeffY[hy]>0)
                        hists[I-1+hx][J-1+hy].put(m*coeffX[hx]*coeffY[hy],alpha);
            //hists[I][J].put(GaussKernelFactory::gaussian->apply(m,i,j),alpha);
        }

     for (int i=0;i<HISTS_COUNT;i++)
         for (int j=0;j<HISTS_COUNT;j++)
         {
             hists[i][j].normalize();
             hists[i][j].crop();
             hists[i][j].normalize();
         }
}

double Descriptor::destination(const Descriptor &descriptor) const
{
    double sum = 0;
    for(int i=0;i<HISTS_COUNT;i++)
        for(int j=0;j<HISTS_COUNT;j++)
    {
        sum += hists[i][j].destination(descriptor.hists[i][j]);
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
        phi-=2*M_PI;
        if (phi<0)
            phi = 0;
    }
    while (phi < 0)
    {
        phi+=2*M_PI;
    }
    double fragment = 2*M_PI/size;
    int iMin = phi/fragment;
    // Интерпляция между корзинами
    double r1 = phi - iMin*fragment;
    double r2 = (1+iMin)*fragment - phi;
    buskets[iMin%size] += r2*m/fragment;
    buskets[(iMin+1)%size] += r1*m/fragment;
}

