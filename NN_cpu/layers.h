#ifndef LAYER_H
#define LAYER_H
#include<iostream> 
#include<random>
#include "matrix.h"

using namespace std;
enum LAYER {linear, relu, sigmoid};

class Layer{
    public:
    string name="name", taip="taip";

    //virtual ~Layer() {}
    //make these virtual
    virtual void forward(){
        cout<<"In the wrong place, this is the placeholder forward"<<endl;
        return;
    }

    virtual void backward(){
        cout<<"In the wrong place, this is the placeholder backward"<<endl;
        return;
    }

    virtual void step(){
        cout<<"In the wrong place, this is the placeholder step"<<endl;
        return;
    }
};

class Linear: public Layer{
    public:
    //make static constant
    int d_out, d_in, bias, A_is_initialized=0;
    Matrix W, dW, dA, A;
    string taip = "Dense Layer", name;
    //Linear(){


    

    //d_out is the dimension of the hidden layer, d_in is the dimension of the previosu layer
    Linear(int d_out, int d_in, string name, int bias=1){
        if(bias!=0 && bias!=1){
            cout<<"Invalid value for bias";
            return;
        }
        if(bias==1){
            d_in+=1;
            //cout<<"din";
        }
        this->name = name;
        this->d_out = d_out;
        this->d_in = d_in;
        this->bias = bias;
        this->W =  Matrix(d_in, d_out);
        this->dW = Matrix(d_in, d_out);
        this->W.rand_init();
        //W.print();
    }

    Matrix forward(Matrix A){
        if((A.w!=W.h && bias==0) || (A.w+1!=W.h && bias==1)){
            
            cout<<endl<<"input and weight dimensions do not match for product ("<<A.h<<","<<A.w<<") ("<<W.h<<","<<W.w<<") Layer "<<name<<endl;
            exit(1);
        }
        //this->A = Matrix(A.h, A.w);
        if(bias==1){
            A = A.add_bias_col();
        }
        this->A = A;
        //A.print();
        //W.print();
        //cout<<"Once";
        return  A.matmul(this->W);
        
    }

    Matrix backward(Matrix dY){
        //make sure this isn't incorrect
        //(this->A).transpose().print();
        //dY.print();
        this->dW = (this->A.transpose()).matmul(dY);
        this->dA = Matrix(this->A.h, this->A.w);
        this->dA = dY.matmul((this->dW.transpose()));
        return this->dA;
    }

    void step(float alpha){
        this->W = this->W.diff(dW.scalar_mul(alpha));

    }
};

class ReLU: public Layer{
    public:
    string taip = "ReLU Activation", name;
    Matrix W, dW, Z;

    ReLU(){
        string name = "default ReLU";
        this->name = name;
    }

    ReLU(string name){
        this->name = name;

    }

    
    double relu_single(double x){

        if(x>=0){
            return x;
        }
        else{
            return 0.d;
        }
    }

    Matrix ReLU_op(Matrix X){
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[i][j] = relu_single(X.elements[i][j]);
            }
        }
    return temp;
    }

    double relu_back_single(double x){

        if(x>=0){
            return 1.d;
        }
        else{
            return 0.d;
        }
    }

    Matrix ReLU_back_op(Matrix X){
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[i][j] = relu_back_single(X.elements[i][j]);
            }
        }
    return temp;
    }


    Matrix forward(Matrix X){
        this->Z = Matrix(X.h, X.w);
        this->Z = X;
        //cout<<"RELU"<<endl;
        //X.print();
        //ReLU_op(X).print();
        //cout<<"RELU"<<endl;
        return ReLU_op(X);
    }

    Matrix backward(Matrix dY){
        
        return dY.hadamard(ReLU_back_op(this->Z));
    }

};

class Sigmoid: public Layer{
    public:
    string taip = "SIGMOID Activation", name;
    Matrix W, dW, Z;

    Sigmoid(){
        string name = "default Sigmoid";
        this->name = name;
    }

    Sigmoid(string name){
        this->name = name;

    }

    
    double sigmoid_single(double x){
        return 1.d/(1 + exp(-x));
    }

    Matrix sigmoid_op(Matrix X){
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[i][j] = sigmoid_single(X.elements[i][j]);
            }
        }
    return temp;
    }

    double sigmoid_back_single(double x){
        return sigmoid_single(x)*(1-sigmoid_single(x));
    }

    Matrix sigmoid_back_op(Matrix X){
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[i][j] = sigmoid_back_single(X.elements[i][j]);
            }
        }
    return temp;
    }


    Matrix forward(Matrix X){
        this->Z = Matrix(X.h, X.w);
        this->Z = X;
        //cout<<"RELU"<<endl;
        //X.print();
        //ReLU_op(X).print();
        //cout<<"RELU"<<endl;
        return sigmoid_op(X);
    }

    Matrix backward(Matrix dY){
        
        return dY.hadamard(sigmoid_back_op(this->Z));
    }

};


#endif