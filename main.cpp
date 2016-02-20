#include <QCoreApplication>
#include <iostream>
#include "smatrimage.h"

#include "zeroreader.h"
#include "torreader.h"
#include "symmetryreader.h"
#include "continuereader.h"

#include "separablekernel.h"
#include "sobelfilter.h"
#include "gausskernelfactory.h"

#include <memory>
using namespace std;



unique_ptr<FilterKernel> getFilter(char filterName)
{
    switch (filterName)
    {
        case 'g':  return GaussKernelFactory::getFilter(5); break;
        case 's':
        default:
            return make_unique<SobelFilter>(3,3);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 0 && argv[0][0] =='?')
    {
        cout << "first parameter is the file name " << endl;
        cout << "modes enabled: " << endl;
        cout << "n - create normalize image" << endl;
        cout << "f - crete filtered image" << endl;
        cout << "p - crete piramid" << endl;
        cout << "border workers enabled: " << endl;
        cout << "z - zeros outside the border" << endl;
        cout << "c - continue border pixels" << endl;
        cout << "t - torus" << endl;
        cout << "s - symmetry" << endl;
        cout << "filters enabled: " << endl;
        cout << "s - Sobel filter" << endl;
        cout << "g - Gauss filter" << endl;
    }
    //QString fileName = "c:/WORK/study/images_data/kirpichnaya_kladka1.jpg";
    QString fileName = "c:/WORK/study/images_data/iris10.jpg";

    if (argc >1)
        fileName = argv[1];
    unique_ptr<SmartImage>  image = make_unique<SmartImage>(fileName);

    char mode = 'f';
    if (argc > 2)
    {
        mode = argv[2][0];
    }
    char readerName = 'z';
    if (argc > 2)
    {
        readerName = argv[2][0];
    }
    image->setBorderType(readerName);

    char filterName = 's';
    if (argc > 3)
    {
        filterName = argv[3][0];
    }
    unique_ptr<FilterKernel> filter;

    switch (mode)
    {
        case 'f':
        {
            filter = getFilter(filterName);
            image->saveFiltered(*filter);
        } break;
        case 'p':
        {
            image->saveNormalize();
            image->savePiramid(3,0.5,1.);
            cout << "value is " << image->L(3,3,3) << endl;
        } break;
        case 'n':
        default: image->saveNormalize();
    }
    cout << "Complete!"<<endl;
    return 0;
}