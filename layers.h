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

    virtual Matrix forward(Matrix& A) = 0;

    virtual Matrix backward(Matrix& A) = 0;

    virtual void step(float alpha) = 0;

    virtual string get_name() = 0;

    virtual string get_taip() = 0;
    

};


//Dense layer
class Linear: public Layer{
    public:

    /*
    d_out:size of layer
    d_in: size of the last dimension of the previous layer
    bias: whether(1) or not(0) a bias term is used
    W,dW: Weights and gradients of weights
    A,dA: input to the layer, itsgradient, used for backpropagation
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

    Matrix forward(Matrix& A){
        /*
        computes forward pass
        input
        A: input from previous layer

        returns
        output = A*W;
        */

        if(A.w!=W.h){
            
            cout<<endl<<"input and weight dimensions do not match for product ("<<A.h<<","<<A.w<<") ("<<W.h<<","<<W.w<<") Layer "<<name<<endl;
            exit(1);
        }

        this->A = A;
        Matrix output = A.matmul(this->W);
        if(this->bias==1){output = output.bias_add(this->b);}
        return  output;
        
    }

    Matrix backward(Matrix& dY){
        /*
        computes backward pass
        input:
        dY: gradient of the next layer

        returns: gradient of what was the input to this layer
        */
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
        /*
        Performs ReLU on a single input, used by ReLU op
        */

        if(x>=0){
            return x;
        }
        else{
            return 0.0;
        }
    }

    Matrix ReLU_op(Matrix &X){
        /*
        Performs ReLU on a matrix
        input
        X: inputs to the layer

        returns
        temp: ReLU on the input
        */
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[temp.w*i + j] = relu_single(X.elements[X.w*i + j]);
            }
        }
    return temp;
    }

    double relu_back_single(double x){
        /*
        Performs ReLU back op on a single input. Used by ReLU back op()
        */

        if(x>=0){
            return 1.0;
        }
        else{
            return 0.0;
        }
    }

    Matrix ReLU_back_op(Matrix &X){
        /*
        Performs ReLU backward op on a matrix
        */
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[temp.w*i + j] = relu_back_single(X.elements[X.w*i + j]);
            }
        }
    return temp;
    }


    Matrix forward(Matrix &X){
        /*
        forward pass
        input
        X: output of the previous layer

        returns
        ReLU(X)
        */
        this->Z = Matrix(X.h, X.w);
        this->Z = X;
        return ReLU_op(X);
    }

    Matrix backward(Matrix &dY){
        /*
        Backward pass over ReLU
        input:
        gradient of previous layer

        returns: for each number, returns the number if the number is greater than 0 else 0
        */
        return dY.hadamard(ReLU_back_op(this->Z));
    }

    void step(float alpha){}

    string get_name(){
        /*
        get the layer's name
        */
        return this->name;
    }
    string get_taip(){
        /*
        get the layer's type
        */
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
        /*
        Perform sigmoid operation over a single number, used by sigmoid_op()
        */
        //cout<<1.d/(1 + exp(-x))<<endl;
        return exp(x)/(1 + exp(x));
    }

    Matrix sigmoid_op(Matrix &X){
        /*
        Perform sigmoid operation over a matrix
        returns the sigmoid of X
        */
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[temp.w*i + j] = sigmoid_single(X.elements[X.w*i + j]);
            }
        }
    return temp;
    }


    double sigmoid_back_single(double x){
        /*
        sigmoid backprop over a single element. Used by sigmoid_back_prop().
        */
        return sigmoid_single(x)*(1-sigmoid_single(x));
    }


    Matrix sigmoid_back_op(Matrix &X){
        /*
        Performs sigmoid back op over a matrix
        */
        Matrix temp = Matrix(X.h, X.w);
        for(int i=0;i<X.h;i++){
            for(int j=0;j<X.w;j++){
                temp.elements[temp.w*i + j] = sigmoid_back_single(X.elements[X.w*i + j]);
            }
        }
    return temp;
    }


    Matrix forward(Matrix& X){
        /*
        forward pass

        input
        X: output of the previous layer

        returns
        sigmoid_op(X)
        */
        this->Z = Matrix(X.h, X.w);
        this->Z = X;
        return sigmoid_op(X);
    }

    Matrix backward(Matrix& dY){
        /*
        backward pass

        input: the gradient from the next layer

        returns:
        elementwise product between the gradient and the sigmoid of what was the input to sigmoid during forward pass 
        */
        return dY.hadamard(sigmoid_back_op(this->Z));
    }

    void step(float alpha){}

    string get_name(){
        /*
        get name
        */
        return this->name;
    }
    string get_taip(){
        /*
        get type
        */
        return this->taip;
    }

};


#endif
