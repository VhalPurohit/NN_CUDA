#ifndef LOSS_H
#define LOSS_H
#include<iostream>
#include "matrix.h"

using namespace std;

//Parent abstract loss class, every loss must have these 2 functions
class Loss{
    public:
    virtual Matrix compute(Matrix Y_hat, Matrix Y) = 0;
    virtual Matrix backward() = 0;

};

class MSE_Loss: public Loss{
    public:

    /*
    Y: ground truth
    Y_hat: predictions
    */
    Matrix Y, Y_hat;

    Matrix compute(Matrix Y_hat, Matrix Y){
        /*
        Computes the MSE, 1/m(y_hat-y)**2
        */
        if(Y.h!=Y_hat.h || Y.w!=Y_hat.w){
                cout<<"Prediction size does not match output size. Prediction size is "<<Y_hat.h<<"x"<<Y_hat.w<<" and ground truth size is "<<Y.h<<"x"<<Y.w<<endl;
                exit(1);
            }
        this->Y_hat = Y_hat;
        this->Y = Y;    
        Matrix temp = Matrix(Y_hat.h, Y_hat.w);
        temp = Y_hat.diff(Y);
        //temp.print();
        temp = temp.power(2).scalar_mul(0.5d/(Y_hat.h));
        return temp;

    }

    Matrix backward(){
        /*
        computes the gradient wuth respect to Y_hat
        */
        Matrix temp = Matrix(this->Y_hat.h, this->Y_hat.w);
        double m = 1.d/(double)(this->Y_hat.h);
        return ((this->Y_hat).diff(this->Y)).scalar_mul(m);
    }

};

#endif