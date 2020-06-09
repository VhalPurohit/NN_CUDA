#ifndef LAYER_H
#define LAYER_H
#include<iostream> 
#include<random>
#include "matrix.h"

using namespace std;
enum LAYER {linear, relu, sigmoid};


//This is the parent abstract class, each class must have these 5 functions to qualify as a layer
class Layer{
    public:
    string name,taip;//="name", taip="taip";

    //virtual ~Layer() {}
    //make these virtual
    virtual Matrix forward(Matrix A) = 0;

    virtual Matrix backward(Matrix A) = 0;

    virtual void step(float alpha) = 0;

    virtual string get_name() = 0;

    virtual string get_taip() = 0;
    
    /*
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
    */
};


//Dense layer
class Linear: public Layer{
    public:
    //make static constant
    /*
    d_out:size of layer
    d_in: size of the last dimension of the previous layer
    bias: whether(1) or not(0) a bias term is used
    W,dW: Weights and gradients of weights
    A,dA: input to the layer, used for backpropagation
    b,db: bias term and its gradient

    */
    int d_out, d_in, bias;
    Matrix W, dW, dA, A, b, db;
    string taip = "Dense Layer", name;
    //Linear(){


    

    //d_out is the dimension of the hidden layer, d_in is the dimension of the previosu layer
    //initializer
    Linear(int d_out, int d_in, string name, int bias=1){
        if(bias!=0 && bias!=1){
            cout<<"Invalid value for bias";
            return;
        }
        this->name = name;
        this->d_out = d_out;
        this->d_in = d_in;
        this->bias = bias;
        this->W =  Matrix(d_in, d_out);
        this->dW = Matrix(d_in, d_out);
        this->W.rand_init();
        this->b = Matrix(1,d_out);
        this->db = Matrix(1,d_out);
        //W.print();
    }

    string get_name(){
        return this->name;
    }
    string get_taip(){
        return this->taip;
    }

    Matrix forward(Matrix A){
        if(A.w!=W.h){
            
            cout<<endl<<"input and weight dimensions do not match for product ("<<A.h<<","<<A.w<<") ("<<W.h<<","<<W.w<<") Layer "<<name<<endl;
            exit(1);
        }

        this->A = A;
        Matrix output = A.matmul(this->W);
        if(this->bias==1){output = output.bias_add(this->b);}
        return  output;
        
    }

    Matrix backward(Matrix dY){
        //dY.print();
        this->dW = (this->A.transpose()).matmul(dY);
        if(this->bias==1){this->db = dY.reduce_axis_0();}
        this->dA = dY.matmul((this->dW.transpose()));
        return this->dA;
    }

    void step(float alpha){
        //dW.print();
        this->W = this->W.diff(this->dW.scalar_mul(alpha));
        if(bias==1){this->b = this->b.diff(this->db.scalar_mul(alpha));}

    }
};

class ReLU: public Layer{
    public:
    /*
    Z: The input to the layer, used for backpropagation 
    */
    string taip = "ReLU Activation", name;
    Matrix Z;

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
        return ReLU_op(X);
    }

    Matrix backward(Matrix dY){
        return dY.hadamard(ReLU_back_op(this->Z));
    }

    void step(float alpha){}

    string get_name(){
        return this->name;
    }
    string get_taip(){
        return this->taip;
    }

};

class Sigmoid: public Layer{
    public:
    /*
    Z: the input to the layer used for backpropagation
    */
    string taip = "SIGMOID Activation", name;
    Matrix Z;

    Sigmoid(){
        string name = "default Sigmoid";
        this->name = name;
    }

    Sigmoid(string name){
        this->name = name;

    }

    
    double sigmoid_single(double x){
        //cout<<1.d/(1 + exp(-x))<<endl;
        return exp(x)/(1 + exp(x));
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
        return sigmoid_op(X);
    }

    Matrix backward(Matrix dY){
        return dY.hadamard(sigmoid_back_op(this->Z));
    }

    void step(float alpha){}

    string get_name(){
        return this->name;
    }
    string get_taip(){
        return this->taip;
    }

};


#endif