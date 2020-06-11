#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
#include<random>
#include<cmath>
#include<ctime>
#include<chrono>
//#include "layers.h"
using namespace std;
//basically, this is the data structure used for all the operations

//template <typename T>
class Matrix{
    public:
    /*
    h is the number of rows and w is the numebr of columns
    elements is a 2d array which stores the actual values
    */
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
        /*
        elementwise matrix addition
        */
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
        /*
        elementwise matrix difference
        */
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
        /*
        naive matrix multiplication
        */
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
        /*
        matrix transpose
        */
        Matrix temp = Matrix(this->w, this->h);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[j][i] = this->elements[i][j];
            }
        }
        return temp;
    }

    Matrix hadamard(Matrix other){
        /*
        elementwise matrix product
        */

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
        /*
        each element in the matrix raised to a power n
        */
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

    Matrix mat_sqrt(){
        /*
        returns a matrix where each is element is the squareroot of the element at the same position in the calling matrix
        */
        Matrix out = Matrix(this->h, this->w);
        //out.init(1.d);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out.elements[i][j] = sqrt(this->elements[i][j]);
                
            }            
        }
    
    return out;
    }

    Matrix calc_means(){
        /*
        calculate the columnwise mean for the matrix, return a (1,this->w) matrix where each element is the 
        //mean of the column ie feature
        */

        Matrix means = Matrix(1,this->w);

        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                means.elements[0][j]+=this->elements[i][j];

            }
        }   

        for(int i=0;i<this->w;i++){
            means.elements[0][i] = means.elements[0][i]/(double)this->h;
        }
    return means;
    }

    Matrix calc_std(){
        /*
        calculate columnwise standard deviation. Returns (1,this->w) matrix where each element is 
        //the standard deviation of the column/feature that that column represents
        */
        Matrix means = this->calc_means();
        Matrix std = Matrix(1,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                std.elements[0][j]+=pow(this->elements[i][j]-means.elements[0][j],2);
            }
        }
        //cout<<"in here";
        //std = std.power(2);
        std = std.scalar_mul(1.d/(double)this->h);
        std = std.mat_sqrt();
        return std;
        
    }

    Matrix m_normalise(){
        /*
        mean normalizes the matrix: makes the mean of each column 0
        */
        Matrix means = this->calc_means();
        //means.print();
        Matrix out = Matrix(this->h,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out.elements[i][j] = this->elements[i][j]-means.elements[0][j];
            }
        
        }
    return out;
    }

    Matrix s_normalise(){
        /*
        std normalizes the matrix, makes the std of each column 1
        */
        Matrix std = this->calc_std();
        //std.print();
        Matrix out = Matrix(this->h,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out.elements[i][j] = this->elements[i][j]/(double)std.elements[0][j];
            }
        
        }
    return out;
    }

    Matrix ms_normalise(){
        /*
        mean and std normalization, make the mean 0 and std 1 for each feature
        */
        Matrix out = Matrix(this->w,this->h);
        out = this->m_normalise();
        out = out.s_normalise();
        return out;
    }

    Matrix bias_add(Matrix bias){
        /*
        defunct: was used when I wanted to make the bias terms part of the weight matrices
        */
        if(this->w!=bias.w){
            cout<<"The width for the matrix ("<<this->w<<") and the bias ("<<bias.w<<") aren't equal";
            exit(1);
        }
        Matrix temp = Matrix(this->h,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[i][j] = this->elements[i][j]+bias.elements[0][j];
            }
        }
        return temp;
    }

    void init(double x){
        /*
        initialize the matrix with element x
        */
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                this->elements[i][j] = x;
            }
        }
    }

    double sum_reduce(){
        /*
        add all the elements is the matrix and return the sum
        */
        double out=0;
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out+=this->elements[i][j];
            }
        }
        return out;
    }

    Matrix reduce_axis_0(){
        /*
        basically sum(axis=0)
        */
        Matrix out = Matrix(1,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out.elements[0][j]+=this->elements[i][j];
            }
        }
        return out;
    }

    Matrix scalar_mul(double n){
        /*
        multiply each element by n
        */
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

    Matrix clip(double m_min, double m_max){
        /*
        used to clip the matrix values between m_min and m_max. Used to solve exploding gradients
        */
        Matrix vec = Matrix(this->h,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                //cout<<i<<j;
                double temp = this->elements[i][j];
                if(temp<m_min){
                    //cout<<"min\n";
                    vec.elements[i][j]=m_min;}

                else if(temp>m_max){
                    //cout<<"max\n";
                    vec.elements[i][j]=m_max;}
                
                else{
                    //cout<<"where\n";
                    vec.elements[i][j]=temp;}
            }
        }
    return vec;
    }

    double** create_array2d(int i, int j){
        /*
        code to create memory for the 2d array
        */
        double **out = new double*[i];
        for(int index=0;index<i;index++){
            out[index] = new double[j];
        }
        return out;
    }

    void print(){
        /*
        pretty prints the matrix
        */
        for(int a=0;a<this->h;a++){
            for(int b=0;b<this->w;b++){
                cout<<this->elements[a][b]<<" ";

            }
        cout<<endl;
        }
    cout<<endl;
    }

    void rand_init(int s=0){
        /*
        random initialization with numbers from random distribution having variance 2/n
        */
        //srand(time(NULL));
        unsigned seed;
        if(s!=0){seed = s;}
        else{seed = std::chrono::system_clock::now().time_since_epoch().count();}
        std::default_random_engine generator(seed);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){

                std::uniform_real_distribution<double> distribution(0,1);
                this->elements[i][j] = distribution(generator)*sqrt(2.d/((double)this->h));
                
                //cout<<this->elements[i][j]<<endl;
            }
        }
    }

};

#endif

