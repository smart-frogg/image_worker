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
    QTransform normalize(vector<Descriptor> *desc);
    void formMatrix(vector<Descriptor*> descs, gsl_matrix *A);
    double checkHypothesis(gsl_matrix *H);
    gsl_matrix *ransac();
    int getRandomPoint();
public:
    vector <Descriptor> *descFrom;
    vector <Descriptor> *descTo;
    Transformer(double treshold);
    QTransform * getTransform();
};

#endif // TRANSFORMER_H
