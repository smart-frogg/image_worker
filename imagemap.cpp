#include "imagemap.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h>
ImageMap::ImageMap(int height, int width)
{
    this->height = height;
    this->width = width;
    this->borderType = 'z';
    data = make_unique<double[]>(height*width);
}

void ImageMap::setData(double value, unsigned int x, unsigned int y)
{
    data[x*height+y] = value;
}
void ImageMap::setBorderType(char borderType)
{
   this->borderType = borderType;
}

double ImageMap::getData(int x, int y) const{
    if (x >=0 && y >=0 && x<width && y<height)
       return data[x*height+y];
    int clearY = y;
    int clearX = x;
    switch (borderType)
    {
        case 'c':
            if (height <= clearY)
                clearY = height - 1;

            if (0 > clearY)
                clearY = 0;

            if (width <= clearX)
                clearX = width - 1;

            if (0 > clearX)
                clearX = 0;
        break;
        case 't':
            clearX = (width + x) % width;
            clearY = (height + y) % height;
        break;
        case 's':
            if (height <= clearY)
                clearY = 2 * height - clearY;

            if (0 > clearY)
                clearY = - clearY;

            if (width <= clearX)
                clearX = 2* width - clearX;

            if (0 > clearX)
                clearX = - clearX;
        break;
        case 'z':
        default: return 0;
    }
    return data[clearX*height+clearY];
}

int ImageMap::getHeight() const {
    return height;
}

int ImageMap::getWidth() const{
    return width;
}

unique_ptr<ImageMap> ImageMap::copy() const
{
    unique_ptr<ImageMap> image = make_unique<ImageMap>(height,width);
    std::copy(data.get(),data.get()+height*width, image->data.get());
    image->setBorderType(borderType);
    return image;
}
unique_ptr<QImage> ImageMap::asImage() {
    unique_ptr<QImage> image = make_unique<QImage>(width, height, QImage::Format_RGB32);
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
        {
            int brightness = (int)(getData(i, j)*255);
            image->setPixel(i,j,qRgb(brightness,brightness,brightness));
        }
    return image;
}

void ImageMap::saveToFile (QString filename){
   // normalize();
    unique_ptr<QImage> image = asImage();
    image->save(filename,"JPG", 100);
}
void ImageMap::normalize()
{
     double* data_ptr = &data[0];
     auto result = minmax_element(data_ptr,data_ptr+height*width);
     cout << *(result.first) << " " << *(result.second) << endl;
     double floor = *(result.second)-*(result.first);
     for(int i=0; i<width; i++)
         for(int j=0; j<height; j++)
         {
             setData((getData(i,j)-*(result.first))/floor,i,j);
             //cout << (getData(i,j)-*(result.first))/floor<<" ";
         }
}
double ImageMap::getAvg()
{
    double* data_ptr = &data[0];
    return accumulate(data_ptr,data_ptr+height*width,0.)/(height*width);
}

void ImageMap::brightness(double k)
{
    for(int i=0; i<width*height; i++)
    {
        data[i] *= k;
        if (data[i]<0)data[i]=0;
        if (data[i]>1) data[i]=1;
    }    //normalize();
}

void ImageMap::kontrast(double k)
{
    double avg = getAvg();
    for(int i=0; i<width*height; i++)
    {
        data[i] += (data[i]-avg)*k;
        if (data[i]<0)data[i]=0;
        if (data[i]>1) data[i]=1;
    }
}

void ImageMap::saltAndPepper(double k)
{
    int count = width*height*(1-k)/200;
    for (int i=0;i<count;i++)
        setData(1,rand()%width,rand()%height);
    for (int i=0;i<count;i++)
        setData(0,rand()%width,rand()%height);
}
