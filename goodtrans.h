#ifndef GOODTRANS_H
#define GOODTRANS_H


class GoodTrans
{
public:
    double treshold;
    QTransform normalize(vector<Descriptor> *desc);
    void formMatrix(Point *p1, Point *p2, gsl_matrix_view A);
    double checkHypothesis(gsl_matrix *H);
    gsl_matrix *ransac();
    int getRandomPoint();
    GoodTrans(double treshold);
};

#endif // GOODTRANS_H
