#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "descriptor.h"

#define GSL_EXPORT
extern "C"{
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
}
#include <QTransform>

class Transformer
{
private:
    double treshold;
    void normalize(vector <Descriptor>* desc);
    void formMatrix(Point *p1, Point *p2, gsl_matrix_view A);
    double checkHypothesis(gsl_vector_view h);
    gsl_vector *ransac();
    int getRandomPoint();
public:
    vector <Descriptor> *descFrom;
    vector <Descriptor> *descTo;
    Transformer(double treshold);
    QTransform * getTransform();
};

#endif // TRANSFORMER_H
