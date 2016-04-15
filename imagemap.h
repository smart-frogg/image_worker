#ifndef IMAGEMAP_H
#define IMAGEMAP_H
#include <memory>
#include <QtGui>
#define BASE_PATH "c:/WORK/study/images_data/"
using namespace std;

class ImageMap
{
private:
    unique_ptr<double[]> data;
    int height;
    int width;
    char borderType = 'z';

public:
    void setData(double d, unsigned int x, unsigned int y);
    double getData(int x, int y) const;
    int getHeight() const;
    int getWidth() const;
    void setBorderType(char border);
    unique_ptr<ImageMap> copy() const;
    ImageMap(int height,int width);
    unique_ptr<QImage> asImage();
    void saveToFile(QString filename);
    void normalize();
    double getAvg();
    void brightness(double k);
    void kontrast(double k);
    void saltAndPepper(double k);
};

#endif // IMAGEMAP_H
