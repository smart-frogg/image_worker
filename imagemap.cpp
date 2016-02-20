#include "imagemap.h"
#include <algorithm>
#include <iostream>
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
    data[clearX*height+clearY];
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
void ImageMap::saveToFile (QString filename){
   // normalize();
    unique_ptr<QImage> image = make_unique<QImage>(width, height, QImage::Format_RGB32);
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
        {
            int brightness = (int)(getData(i, j)*255);
            image->setPixel(i,j,qRgb(brightness,brightness,brightness));
        }
    image->save(filename,"JPG");
}
void ImageMap::normalize()
{
     auto data_ptr = data.get();
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