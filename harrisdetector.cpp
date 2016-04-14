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
Histogram::Histogram()
{
    for(int i=0;i<BUSKETS_COUNT;i++)
        buskets[i]=0;
}

void HarrisDetector::calcDescriptors()
{
    vector<Point> pts = isClear ? getPoints(): points;
    int i=0;
    for(Point p:pts)
    {
        descriptors.push_back(Descriptor(p,sobelFilter.get()));
        i++;
    }
}

vector<Descriptor> *HarrisDetector::getDescriptors()
{
    return &descriptors;
}

Descriptor::Descriptor(const Point &point, SobelFilter *sobelFilter)
{
    this->point = Point(point);
    this->sobelFilter = sobelFilter;
    clothest=NULL;
    calc(0,-4,-4,4);
    calc(1,0,-4,4);
    calc(2,-4,0,4);
    calc(3,0,0,4);
}

double Descriptor::destination(Descriptor descriptor)
{
    double sum = 0;
    for(int i=0;i<4;i++)
    {
        sum += hists[i].destination(descriptor.hists[i]);
    }
    //cout<<sum<<endl;
    return sum;
}

double Histogram::destination(Histogram hist)
{
    double sum = 0;
    for(int i=0;i<BUSKETS_COUNT;i++)
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
    cout<<minDestination<<endl;
}

void HarrisDetector::saveCompare(QString filename, ImageMap *data2)
{
    unique_ptr<QImage> image = this->data->asImage();
    unique_ptr<QImage> image2 = data2->asImage();
    unique_ptr<QImage> result = make_unique<QImage>(this->data->getWidth()+data2->getWidth(), max(this->data->getHeight(),data->getHeight()), QImage::Format_RGB32);
    unique_ptr<QPainter> painter = make_unique<QPainter>(result.get());
    painter->drawImage(0,0,*image);
    painter->drawImage(this->data->getWidth(),0,*image2);
    painter->setBrush(Qt::green);

    vector<Descriptor> *descs = getDescriptors();
    for(Descriptor &d:*descs)
    {
        painter->setPen(qRgb(rand()%255,rand()%255,rand()%255));
        Point *p1 = d.getPoint();
        Point *p2 = d.getClothest()->getPoint();
        painter->drawLine(p1->x,p1->y,p2->x + this->data->getWidth(),p2->y);
        painter->drawEllipse(p1->x-2,p1->y-2,5,5);
        painter->drawEllipse(p2->x-2 + this->data->getWidth(),p2->y-2,5,5);
        //image->setPixel(p.x,p.y,qRgb(255,0,0));
    }
    result->save(filename,"JPG", 100);
}

void Descriptor::calc(int histNum, int startX, int startY, int size)
{
    ImageMap *dX = sobelFilter->getImageX();
    ImageMap *dY = sobelFilter->getImageY();
    for(int i=point.x+startX;i<point.x+startX+size;i++)
        for(int j=point.y+startY;j<point.y+startY+size;j++)
        {
            hists[histNum].put(dX->getData(i,j),dY->getData(i,j));
        }
     hists[histNum].normalize();
     hists[histNum].crop();
     hists[histNum].normalize();
}

Point* Descriptor::getPoint()
{
    return &point;
}

Descriptor* Descriptor::getClothest()
{
    return clothest;
}


void Histogram::normalize()
{
    auto result = minmax_element(buskets,buskets+BUSKETS_COUNT);
    double floor = *(result.second)-*(result.first);
    if (floor<0.0000001)
        return;
    for (int i=0;i<BUSKETS_COUNT;i++)
    {
        buskets[i] = (buskets[i]-*(result.first))/floor;
    }
}
void Histogram::crop()
{
    for (int i=0;i<BUSKETS_COUNT;i++)
    {
        if (buskets[i]>0.2) buskets[i] = 0.2;
    }
}

void Histogram::put(double dX, double dY)
{
    double phi = atan2(dY,dX);
    if (phi<0)
        phi+=2*M_PI;
    double m = sqrt(dX*dX+dY*dY);
    double fragment = 2*M_PI/(BUSKETS_COUNT);
    int iMin = phi/fragment;
    double r1 = phi - iMin*fragment;
    double r2 = (1+iMin)*fragment - phi;
   // cout <<r1<<" "<<r2<<endl;
    buskets[iMin] += r2*m/fragment;
    buskets[iMin+1] += r1*m/fragment;
}
