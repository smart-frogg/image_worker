#include "piramid.h"
#include "gausskernelfactory.h"
#include <algorithm>
#include <iostream>

Piramid::Piramid(int countLayers, double sigma0, double sigmaFirst, const ImageMap &input)
{
    this->countLayers = countLayers;
    this->sigma0 = sigma0;
    this->sigmaFirst = sigmaFirst;

    calculateOctavs(input);
}

void Piramid::calculateOctavs(const ImageMap &input)
{
    int size = min(input.getHeight(),input.getWidth());

    double sigma = sqrt(sigmaFirst*sigmaFirst - sigma0*sigma0);
    unique_ptr<FilterKernel> filter = GaussKernelFactory::getFilter(sigma);
    unique_ptr<ImageMap> curr = filter->apply(input);
    while (size > MIN_SIZE)
    {
        octavs.push_back(make_unique<Octave>(countLayers, sigmaFirst, *curr));
        const auto& octave = octavs.back();
        curr = octave->scale();
        size = min(curr->getHeight(),curr->getWidth());
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

double Piramid::L(int x,int y,double sigma)
{
    int k = ceil(log2(sigma/sigmaFirst)) - 1;
    if (k < 0) k = 0;
    if (k >= octavs.size()) k = octavs.size()-1;
    cout << "octave is " << k <<endl;
    int p = 1;
    for (int i=0;i<k;i++)
        p*=2;
    return octavs[k]->L(x/p, y/p, sigma/p);
}
