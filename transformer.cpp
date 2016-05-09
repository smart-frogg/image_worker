#include "transformer.h"
#include <math.h>
#include <iostream>


Transformer::Transformer(double treshold)
{
    this->treshold = treshold;
}

void Transformer::setDescs(vector<Descriptor> *desc)
{
    for (Descriptor &d:*desc)
    {
        if (!d.used) continue;
        descTo.push_back(&d);
        descFrom.push_back(d.getClothest());
    }

}

void Transformer::setDescs(vector<Descriptor*> desc)
{
    for (Descriptor *d:desc)
    {
        if (!d->used) continue;
        descTo.push_back(d);
        descFrom.push_back(d->getClothest());
    }

}

QTransform Transformer::normalize(vector<Descriptor*> *desc)
{
    double maxX = -1;
    double maxY = -1;
    double minX = -1;
    double minY = -1;
    for (auto d:*desc)
    {
        if (minX==-1 || minX>d->getPoint()->x)
            minX=d->getPoint()->x;
        if (minY==-1 || minY>d->getPoint()->y)
            minY=d->getPoint()->y;
        if (maxX<d->getPoint()->x)
            maxX=d->getPoint()->x;
        if (maxY<d->getPoint()->y)
            maxY=d->getPoint()->y;
    }
    for (auto d:*desc)
    {
        d->getPoint()->normX = d->getPoint()->x;// 2*(d.getPoint()->x - minX)/(maxX - minX) - 1;
        d->getPoint()->normY = d->getPoint()->y;//2*(d.getPoint()->y - minY)/(maxY - minY) - 1;
        //d->getPoint()->normX =  2*(d->getPoint()->x - minX)/(maxX - minX) - 1;
        //d->getPoint()->normY = 2*(d->getPoint()->y - minY)/(maxY - minY) - 1;
    }
    QTransform transform;
    transform.translate(-minX, -minY);
    transform.scale(2/(maxX - minX), 2/(maxY - minY));
    transform.translate(-1, -1);
    return transform;
}

void Transformer::formMatrix(vector<Descriptor*> descs, gsl_matrix *A)
{
    int i=0;
    gsl_matrix_set_zero(A);
    for (Descriptor *d:descs)
    {
        Point *p1 = d->getPoint();
        Point *p2 = d->getClothest()->getPoint();
        gsl_matrix_set(A,i+0,0,p1->normX);
        gsl_matrix_set(A,i+0,1,p1->normY);
        gsl_matrix_set(A,i+0,2,1);

        gsl_matrix_set(A,i+1,3,p1->normX);
        gsl_matrix_set(A,i+1,4,p1->normY);
        gsl_matrix_set(A,i+1,5,1);

        gsl_matrix_set(A,i+0,6,-p2->normX*p1->normX);
        gsl_matrix_set(A,i+0,7,-p2->normX*p1->normY);
        gsl_matrix_set(A,i+0,8,-p2->normX);

        gsl_matrix_set(A,i+1,6,-p2->normY*p1->normX);
        gsl_matrix_set(A,i+1,7,-p2->normY*p1->normY);
        gsl_matrix_set(A,i+1,8,-p2->normY);
        i+=2;
    }
}
void Transformer::afinFormMatrix(vector<Descriptor*> descs, gsl_matrix *A)
{
    int i=0;
    gsl_matrix_set_zero(A);
    for (Descriptor *d:descs)
    {
        Point *p1 = d->getPoint();
        Point *p2 = d->getClothest()->getPoint();
        gsl_matrix_set(A,i+0,0,p1->normX);
        gsl_matrix_set(A,i+0,1,p1->normY);
        gsl_matrix_set(A,i+0,2,1);

        gsl_matrix_set(A,i+1,3,p1->normX);
        gsl_matrix_set(A,i+1,4,p1->normY);
        gsl_matrix_set(A,i+1,5,1);

        i+=2;
    }
}

double Transformer::checkHypothesis(gsl_vector_view h)
{
    int positives = 0;
    int used = 0;
    double coef[9];
    for(int i=0;i<9;i++)
        coef[i] = gsl_vector_get(&(h.vector),i);
    positiveDescs.clear();
    for(Descriptor *d:descFrom)
    {
        if (!d->used) continue;
       // cout<<"Used descriptor "<<used<<endl;
        used++;
        Point *p1 = d->getPoint();
        Point *p2 = d->getClothest()->getPoint();
        double x = (coef[0]*p1->x + coef[1]*p1->y + coef[2]) / (coef[6]*p1->x + coef[7]*p1->y + coef[8]);
        double y = (coef[3]*p1->x + coef[4]*p1->y + coef[5]) / (coef[6]*p1->x + coef[7]*p1->y + coef[8]);
        double l = sqrt((x - p2->x)*(x - p2->x) + (y - p2->y)*(y - p2->y)) ;

        //cout << l <<endl;
        if (l < treshold)
        {
            positives++;
            positiveDescs.push_back(d);

        }
    }
    return positives*1.0/used;
}

double Transformer::afinCheckHypothesis(gsl_vector_view h)
{
    int positives = 0;
    int used = 0;
    double coef[9];
    for(int i=0;i<6;i++)
        coef[i] = gsl_vector_get(&(h.vector),i);
    positiveDescs.clear();
    for(Descriptor *d:descFrom)
    {
        if (!d->used) continue;
       // cout<<"Used descriptor "<<used<<endl;
        used++;
        Point *p1 = d->getPoint();
        Point *p2 = d->getClothest()->getPoint();
        double x = (coef[0]*p1->x + coef[1]*p1->y + coef[2]) ;
        double y = (coef[3]*p1->x + coef[4]*p1->y + coef[5]) ;
        double l = sqrt((x - p2->x)*(x - p2->x) + (y - p2->y)*(y - p2->y)) ;

        cout << l <<"\t"<<treshold<<endl;
        if (l < treshold)
        {
            positives++;
            positiveDescs.push_back(d);

        }
    }
    return positives*1.0/used;
}

int Transformer::getRandomPoint(int n1,int n2,int n3)
{
     int n = rand() % descFrom.size();
     while (!descFrom[n]->used || descFrom[n]->getClothest() == NULL || n==n1 ||n==n2 ||n==n3)
         n = rand() % descFrom.size();
     return n;
}

void mul(gsl_matrix *A,gsl_matrix *C)
{
    for(int x=0;x<A->size2;x++)
        for(int y=0;y<A->size2;y++)
        {
            double sum = 0;
            for(int i=0;i<A->size1;i++)
            {
                sum += gsl_matrix_get(A,i,x)*gsl_matrix_get(A,i,y);
            }
            gsl_matrix_set(C,x,y,sum);
        }
}

void Transformer::ransac(double *result,int iterations)
{
    double positives = 0;
    double A_arr[8*9];
    gsl_matrix_view A = gsl_matrix_view_array(A_arr,8,9); // матрица А Ж)
   // double AT_arr[8*9];
   // gsl_matrix_view AT = gsl_matrix_view_array(AT_arr,9,8);
    gsl_matrix *B = gsl_matrix_alloc(9,9);
    gsl_matrix *V = gsl_matrix_alloc(9,9);
    double S_arr[9];
    gsl_vector_view S = gsl_vector_view_array(S_arr,9);

    for(int i=0; i<iterations; i++)
    {
        vector<Descriptor *> descs;

        int n0 = getRandomPoint(-1,-1,-1);
        descs.push_back(descFrom[n0]);
        int n1 = getRandomPoint(n0,-1,-1);
        descs.push_back(descFrom[n1]);
        int n2 = getRandomPoint(n0,n1,-1);
        descs.push_back(descFrom[n2]);
        int n3 = getRandomPoint(n0,n1,n2);
        descs.push_back(descFrom[n3]);
        formMatrix(descs, &(A.matrix));

        //gsl_matrix_transpose_memcpy (&(AT.matrix), &(A.matrix));

        //gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1, &(AT.matrix), &(A.matrix), 0, B);

        mul (&(A.matrix), B);
        gsl_linalg_SV_decomp_jacobi(B, V, &(S.vector));
        gsl_vector_view h = gsl_matrix_column(V,8);
        gsl_vector_scale(&(h.vector),1/gsl_vector_get(&(h.vector),8));


        double curPositives = checkHypothesis(h);
        if (positives < curPositives)
        {
            for (int i=0;i<3;i++)
            {
                for (int j=0;j<3;j++)
                {
                    cout<<gsl_vector_get(&(h.vector),j*3+i)<<"\t";
                }
                cout<<endl;

            }
            for (int i=0;i<9;i++)
                result[i] = gsl_vector_get(&(h.vector),i);
            positives = curPositives;
            cout<< "positives " << positives << endl;
        }

    }
    /*gsl_matrix *A_good = gsl_matrix_alloc(2*positiveDescs.size(),9);
    formMatrix(positiveDescs, A_good);
    mul (A_good, B);
    gsl_linalg_SV_decomp_jacobi(B, V, &(S.vector));
    gsl_vector_view h = gsl_matrix_column(V,8);
    gsl_vector_scale(&(h.vector),1/gsl_vector_get(&(h.vector),8));
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
            cout<<gsl_vector_get(&(h.vector),j*3+i)<<"\t";
        }
        cout<<endl;
    }

    gsl_matrix_free(A_good);*/
    gsl_matrix_free(B);
    gsl_matrix_free(V);

}

void Transformer::afinn(double *result, int iterations)
{
    double positives = 0;
    double A_arr[6*6];
    gsl_matrix_view A = gsl_matrix_view_array(A_arr,6,6); // матрица А Ж)
    gsl_matrix *B = gsl_matrix_alloc(6,6);
    gsl_matrix *V = gsl_matrix_alloc(6,6);
    double S_arr[6];
    gsl_vector_view S = gsl_vector_view_array(S_arr,6);

   // for(int i=0; i<iterations; i++)
    {
        vector<Descriptor *> descs;

        int n0 = getRandomPoint(-1,-1,-1);
        descs.push_back(descFrom[n0]);
        int n1 = getRandomPoint(n0,-1,-1);
        descs.push_back(descFrom[n1]);
        int n2 = getRandomPoint(n0,n1,-1);
        descs.push_back(descFrom[n2]);
        afinFormMatrix(descs, &(A.matrix));

        //gsl_matrix_transpose_memcpy (&(AT.matrix), &(A.matrix));

        //gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1, &(AT.matrix), &(A.matrix), 0, B);

        mul (&(A.matrix), B);
        gsl_linalg_SV_decomp_jacobi(B, V, &(S.vector));
        gsl_vector_view h = gsl_matrix_column(V,5);
        //double curPositives = afinCheckHypothesis(h);
       // if (positives < curPositives)
        {
            for (int i=0;i<6;i++)
            {
                result[i] = gsl_vector_get(&(h.vector),i);
                cout<<result[i]<<endl;
            }
           // positives = curPositives;
          //  cout<< "positives " << positives << endl;
        }

    }

    gsl_matrix_free(B);
    gsl_matrix_free(V);

}

vector<QTransform> Transformer::getTransformVec()
{
    QTransform transform1 = normalize(&descFrom);
    QTransform transform2 = normalize(&descTo);
    double result[9];
    ransac(result,20000);
    vector<QTransform> v;
    QTransform transform(
                result[0],result[3],result[6],
                result[1],result[4],result[7],
                -result[2],-result[5],result[8]
                );
    v.push_back (transform);
    transform = QTransform(
                    result[0],result[3],result[6],
                    result[1],result[4],result[7],
                    result[2],result[5],result[8]
                    );


    v.push_back (transform);
    transform = QTransform(
                    result[1],result[4],result[7],
                    result[0],result[3],result[6],
                    result[2],result[5],result[8]
                    );
    v.push_back (transform);

    return v;
}
QTransform Transformer::getTransform()
{
    QTransform transform1 = normalize(&descFrom);
    QTransform transform2 = normalize(&descTo);
    double result[9];
    ransac(result,20000);
    QTransform transform(
                result[0],result[3],result[6],
                result[1],result[4],result[7],
                result[2],result[5],result[8]
                );

    return transform;
}
QTransform Transformer::getAffinTransform()
{
    QTransform transform1 = normalize(&descFrom);
    QTransform transform2 = normalize(&descTo);
    double result[6];
    afinn(result,20);
    QTransform transform(
                result[0],result[3],0,
                result[1],result[4],0,
                result[2],result[5],1
                );

    return transform;
}
