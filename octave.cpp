#include "octave.h"
#include "config.h"
#include "gausskernelfactory.h"
#include <math.h>
#include <iostream>

Octave::Octave(int countLayers, double sigma, int scale, const ImageMap &input)
{
    this->countLayers = countLayers;
    this->sigmaFirst = sigma;
    this->scaleSize = scale;
    this->height = input.getHeight();
    this->width = input.getWidth();
    calculateLayers(input);
}

double Octave::getStep()
{
    return pow(2,1./countLayers);
}
unique_ptr<QImage> Octave::asImage()
{
    return layers[0]->asImage();
}

void Octave::calculateLayers(const ImageMap &input)
{
    double curSigma = sigmaFirst;
    double step = getStep();
    layers.push_back(input.copy());
    for (int i=0; i<countLayers+3; i++)
    {
        unique_ptr<FilterKernel> filter = GaussKernelFactory::getFilter(sqrt(step*step-1)*curSigma);
        layers.push_back(filter->apply(*layers[i]));
        curSigma*=step;
    }
}
void Octave::genDescriptors(vector<Descriptor> *descriptors, double sigma)
{
    double curSigma = sigma;
    double littleSigma = 1;
    double step = getStep();
    for (int i=0; i<countLayers+3; i++)
    {
        unique_ptr<HarrisDetector> harrisDetector = make_unique<HarrisDetector>(layers[i].get());
        harrisDetector->configure(0.01,0.06,1);
        harrisDetector->detect();
        harrisDetector->clear(100);
        harrisDetector->calcDescriptors(curSigma,littleSigma,scaleSize,descriptors);
        //harrisDetector->calcDescriptors(curSigma);
        curSigma *= step;
        littleSigma *= step;
        //harrisDetector->save(BASE_PATH+QString::number(scaleSize)+"_"+QString::number(i)+"_HarrisonDetector.jpg");
    }
}

unique_ptr<ImageMap> Octave::scale()
{
    const auto& lastImage = layers[countLayers-1];
    int scaleHeight = height / 2;
    int scaleWidth = width / 2;
    unique_ptr<ImageMap> scaleImage = make_unique<ImageMap>(scaleHeight,scaleWidth);
    for (int i = 0; i < scaleWidth; i++)
        for (int j = 0; j < scaleHeight; j++)
        {
            scaleImage->setData(lastImage->getData(2*i,2*j),i,j);
        }
    return scaleImage;
}

int Octave::size()
{
   return layers.size();
}

void Octave::save(QString filename)
{
    int i=0;
    for (const auto& layer : layers)
    {
        layer->saveToFile(filename+"_"+QString::number(i)+".jpg");
        //cout << layer->getAvg() << endl;
        i++;
    }
}
void Octave::genDOG()
{
    for (int i=1;i<layers.size();i++)
    {
        dogs.push_back(genOneDOG(i));
    }
}

unique_ptr<ImageMap> Octave::genOneDOG(int i)
{
    unique_ptr<ImageMap> image = make_unique<ImageMap>(layers[0]->getHeight(),layers[0]->getWidth());
    for(int x=0;x<layers[i]->getWidth();x++)
        for(int y=0;y<layers[i]->getHeight();y++)
        {
            double value = layers[i]->getData(x,y)-layers[i-1]->getData(x,y);
            image->setData(value,x,y);
        }
    //image->saveToFile(BASE_PATH+QString::number(scaleSize)+"_"+QString::number(i)+"_DOG.jpg");
    return image;
}

void Octave::saveDOG(QString filename)
{
    unique_ptr<ImageMap> image = make_unique<ImageMap>(layers[0]->getHeight(),layers[0]->getWidth());
    for (int i=1;i<layers.size();i++)
    {
        for(int x=0;x<layers[0]->getWidth();x++)
            for(int y=0;y<layers[0]->getHeight();y++)
            {
                double value = layers[i]->getData(x,y)-layers[i-1]->getData(x,y);
                image->setData(value,x,y);
            }
        image->normalize();
        image->saveToFile(filename+"_"+QString::number(i)+"_DOG.jpg");
    }
}
double Octave::L(int x,int y,double sigma)
{
    double step = getStep();
    int k = ceil(log(sigma/sigmaFirst)/log(step)) - 1;
    if (k < 0) k = 0;
    if (k >= layers.size()) k = layers.size()-1;
    //cout << "layer in octave is " << k <<endl;
    return layers[k]->getData(x,y);
}
double Octave::dogL(int x,int y,double sigma)
{
    double step = getStep();
    int k = ceil(log(sigma/sigmaFirst)/log(step)) - 1;
    if (k < 0) k = 0;
    if (k >= dogs.size()) k = dogs.size()-1;
    //cout << "layer in octave is " << k <<endl;
    return dogs[k]->getData(x,y);
}
