#include "imagereader.h"

ImageReader::ImageReader()
{

}

void ImageReader::setData(unique_ptr<ImageMap> *data)
{
    this->image = data;
}

int ImageReader::getHeight()
{
    return (*(this->image))->getHeight();
}

int ImageReader::getWidth()
{
    return (*(this->image))->getWidth();
}
