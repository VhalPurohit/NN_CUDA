#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
#include<random>
#include<cmath>
#include<ctime>
#include<chrono>
#include "layers.h"
using namespace std;

//template <typename T>
class Matrix{
    public:
    int h,w;
    double** elements;

    Matrix(){

    }

    Matrix(int i, int j){
        h=i;
        w=j;
        elements = create_array2d(h,w);

    }

    //template <typename T>
    Matrix add(Matrix other){
        if(this->h!=other.h || this->w!=other.w){
            cout<<"Matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<"Matrix sum not possible.\n";
            exit(1);
            
        }
        Matrix temp = Matrix(other.h, other.w);
        for(int i=0;i<other.h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[i][j] = this->elements[i][j] + other.elements[i][j];
            }
        }

        return temp;
    }

    //template <typename T>
    Matrix diff(Matrix other){
        if(this->h!=other.h || this->w!=other.w){
            cout<<"Matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Matrix diff not possible.\n";
            exit(1);
            
        }
        
        Matrix temp = Matrix(other.h, other.w);
        for(int i=0;i<other.h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[i][j] = this->elements[i][j] - other.elements[i][j];
            }
        }

        return temp;
    }

    Matrix matmul(Matrix other){
        if(this->w!=other.h){
            cout<<"height and width do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Matrix product not possible.\n";
            exit(1);

        }
        Matrix temp = Matrix(this->h, other.w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[i][j]=0;
                for(int k=0;k<this->w;k++){
                    temp.elements[i][j]+=this->elements[i][k]*other.elements[k][j];
                }
            }
        }
        return temp;
    }

    Matrix transpose(){
        Matrix temp = Matrix(this->w, this->h);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[j][i] = this->elements[i][j];
            }
        }
        return temp;
    }

    Matrix hadamard(Matrix other){
        if(this->h!=other.h || this->w!=other.w){
            cout<<"matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Hadamard product not possible\n";
            exit(1);
        }
        Matrix temp = Matrix(this->h, this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[i][j] = this->elements[i][j]*other.elements[i][j];
            }
        }
        return temp;
    }

    Matrix power(int n){
        Matrix out = Matrix(this->h, this->w);
        out.init(1.d);
        for(int i=0;i<n;i++){
            for(int i=0;i<this->h;i++){
                for(int j=0;j<this->w;j++){
                    out.elements[i][j] = out.elements[i][j]*this->elements[i][j];
                }
            }            
        }
    
    return out;
    }

    void init(double x){
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                this->elements[i][j] = x;
            }
        }
    }

    double sum_reduce(){
        double out=0;
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out+=this->elements[i][j];
            }
        }
        return out;
    }

    Matrix scalar_mul(double n){
        Matrix temp = Matrix(this->h, this->w);

        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[i][j] = this->elements[i][j]*n;
            }
        }
    return temp;
    }

    //used to add the bias as a part of the weights.
    //A columns of ones is prepended to the input
    Matrix add_bias_col(){
        Matrix temp = Matrix(this->h,this->w+1);
        for(int i=0;i<temp.h;i++){
            for(int j=0;j<temp.w;j++){
                if(j==0){
                    temp.elements[i][j]=1.d;
                }
                else{
                    temp.elements[i][j] = this->elements[i][j-1];
                }
            }
        }
    return temp;
    }

    double** create_array2d(int i, int j){
        double **out = new double*[i];
        for(int index=0;index<i;index++){
            out[index] = new double[j];
        }
        return out;
    }

    void print(){
        for(int a=0;a<this->h;a++){
            for(int b=0;b<this->w;b++){
                cout<<this->elements[a][b]<<" ";

            }
        cout<<endl;
        }
    cout<<endl;
    }

    void rand_init(){
        //srand(time(NULL));
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){

                std::uniform_real_distribution<double> distribution((float)this->w - 1,(float)this->w);
                this->elements[i][j] = distribution(generator);//*sqrt(2.d/((double)this->w - 1));

            }
        }
    }

};

#endif

