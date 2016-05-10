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
            return make_unique<SobelFilter>();
    }
}

int main(int argc, char *argv[])
{

    if (argc > 0 && argv[0][0] =='?')
    {
        cout << "first parameter is the file name " << endl;
        cout << "modes enabled: " << endl;
        cout << "n - create normalize image" << endl;
        cout << "f - create filtered image" << endl;
        cout << "p - create piramid" << endl;
        cout << "d - apply detector" << endl;
        cout << "border workers enabled: " << endl;
        cout << "z - zeros outside the border" << endl;
        cout << "c - continue border pixels" << endl;
        cout << "t - torus" << endl;
        cout << "s - symmetry" << endl;
        cout << "filters enabled: " << endl;
        cout << "s - Sobel filter" << endl;
        cout << "g - Gauss filter" << endl;
        cout << "point detectors enabled: " << endl;
        cout << "m - Moravek" << endl;
        cout << "h - Harris" << endl;
    }
    //QString fileName = "c:/WORK/study/images_data/kirpichnaya_kladka1.jpg";
    QString fileName = "../images_data/rudbekia.jpg";
   // QString fileName = "c:/WORK/study/images_data/Izyashhnyy-cvetok-iris.jpg";
    //QString fileName = "c:/WORK/study/images_data/Iris_sanguinea_01.jpg";

    if (argc >1)
        fileName = argv[1];
    unique_ptr<SmartImage>  image; //= make_unique<SmartImage>(fileName);

    char mode = 's';
    if (argc > 2)
    {
        mode = argv[2][0];
    }
    char readerName = 'c';
    if (argc > 2)
    {
        readerName = argv[2][0];
    }
    //image->setBorderType(readerName);

    char filterName = 'h';
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
            image->saveFiltered(filter.get());
        } break;
        case 'p':
        {
           // image->saveNormalize();
           // image->savePiramid(6,0.5,1.);
            image->genDescriptorsPiramid();
           // cout << "value is " << image->L(3,3,3) << endl;
        } break;
        case 'd':
        {
             image->testDetect(filterName);
        } break;
        case 'B':
        {
            unique_ptr<SmartImage> image1 = make_unique<SmartImage>("C:/WORK/study/images_data/parts/full_big_s.jpg");
            image1->genDescriptorsPiramid();

        } break;

        case 'c':
        {
            unique_ptr<SmartImage> image1 = make_unique<SmartImage>("C:/WORK/study/images_data/parts/lemur.jpg");
            unique_ptr<SmartImage> image2 = make_unique<SmartImage>("C:/WORK/study/images_data/parts/lemur1.jpg");
            image1->compare(image2.get());

        } break;
        case 'b':
        {
            unique_ptr<SmartImage> image1 = make_unique<SmartImage>("../images_data/p1s.JPG");
            unique_ptr<SmartImage> image2 = make_unique<SmartImage>("../images_data/p2s.JPG");
            image1->bind(image2.get());

        } break;
        case 's':
        {
            //unique_ptr<SmartImage> image1 = make_unique<SmartImage>("../images_data/parts/lemur.JPG");
            //unique_ptr<SmartImage> image2 = make_unique<SmartImage>("../images_data/parts/full_big_s.JPG");
            unique_ptr<SmartImage> image1 = make_unique<SmartImage>("C:/WORK/study/images_data/parts/lemur.JPG");
            unique_ptr<SmartImage> image2 = make_unique<SmartImage>("C:/WORK/study/images_data/parts/f.JPG");
            image1->search(image2.get());
        } break;
        case 'n':
        default: image->saveNormalize();
    }
    cout << "Complete!"<<endl;
    return 0;
}
