#ifndef LOSS_H
#define LOSS_H
#include<iostream>
#include "matrix.h"

using namespace std;
class Loss{
    int temp;

};

class MSE_Loss: public Loss{
    public:
    double loss_val;
    Matrix Y, Y_hat;

    MSE_Loss(){
    }

    Matrix compute(Matrix Y_hat, Matrix Y){
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
        Matrix temp = Matrix(this->Y_hat.h, this->Y_hat.w);
        double m = 1.d/(double)(this->Y_hat.h);
        return ((this->Y_hat).diff(this->Y)).scalar_mul(m);
    }

};

#endif