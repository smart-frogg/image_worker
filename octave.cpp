#include "octave.h"
#include "gausskernelfactory.h"
#include <math.h>
#include <iostream>

Octave::Octave(int countLayers, double sigma, const ImageMap &input)
{
    this->countLayers = countLayers;
    this->sigmaFirst = sigma;
    this->height = input.getHeight();
    this->width = input.getWidth();
    calculateLayers(input);
}

double Octave::getStep()
{
    return pow(2,1./countLayers);
}

void Octave::calculateLayers(const ImageMap &input)
{
    double curSigma = sigmaFirst;
    double step = getStep();
    layers.push_back(input.copy());
    for (int i=0; i<countLayers; i++)
    {
        curSigma *= step;
        unique_ptr<FilterKernel> filter = GaussKernelFactory::getFilter(curSigma);
        layers.push_back(filter->apply(input));
    }

}

unique_ptr<ImageMap> Octave::scale()
{
    const auto& lastImage = layers.back();
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

void Octave::save(QString filename)
{
    int i=0;
    for (const auto& layer : layers)
    {
        layer->saveToFile(filename+"_"+QString::number(i)+".jpg");
        i++;
    }
}
double Octave::L(int x,int y,double sigma)
{
    double step = getStep();
    int k = ceil(log(sigma/sigmaFirst)/log(step)) - 1;
    if (k < 0) k = 0;
    if (k >= layers.size()) k = layers.size()-1;
    cout << "layer in octave is " << k <<endl;
    return layers[k]->getData(x,y);
}
