//reference: https://github.com/stevenireeves/ams148
#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
#include<random>
#include<cmath>
#include<ctime>
#include<chrono>
#include "matrix_gpu.h"
//#include "layers.h"
using namespace std;
//basically, this is the data structure used for all the operations

//template <typename T>
class Matrix{
    public:
    /*
    h is the number of rows and w is the numebr of columns
    elements is a 2d array which stores the actual values, basically a 1D array with 2D indexing
    */
    int h,w;
    double* elements;
    int gpu=0;
    

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
        elementwise matrix addition, A+B = A.add(B);
        returns A+B
        */
        if(this->h!=other.h || this->w!=other.w){
            cout<<"Matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<"Matrix sum not possible.\n";
            exit(1);
            
        }
        Matrix temp = Matrix(other.h, other.w);
        for(int i=0;i<other.h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[other.w*i + j] = this->elements[other.w*i + j] + other.elements[other.w*i + j];
            }
        }

        return temp;
    }


    //template <typename T>
    Matrix diff_cpu(Matrix other){
        /*
        elementwise matrix difference. A-B = A.diff(B)
        returns A-B
        */
        if(this->h!=other.h || this->w!=other.w){
            cout<<"Matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Matrix diff not possible.\n";
            exit(1);
            
        }
        
        Matrix temp = Matrix(other.h, other.w);
        for(int i=0;i<other.h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[other.w*i + j] = this->elements[other.w*i + j] - other.elements[other.w*i + j];
            }
        }

        return temp;
    }


    Matrix matmul_cpu(Matrix other){
        /*
        naive matrix multiplication:  A times B = A.matmul(B)
        returns A times B
        */
        if(this->w!=other.h){
            cout<<"height and width do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Matrix product not possible.\n";
            exit(1);

        }
        Matrix temp = Matrix(this->h, other.w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<other.w;j++){
                temp.elements[temp.w*i + j]=0;
                for(int k=0;k<this->w;k++){
                    temp.elements[temp.w*i + j]+=this->elements[this->w*i + k]*other.elements[other.w*k + j];
                }
            }
        }
        return temp;
    }

    Matrix transpose_cpu(){
        /*
        matrix transpose
        returns A.T
        */
        Matrix temp = Matrix(this->w, this->h);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[temp.w*j + i] = this->elements[this->w*i + j];
            }
        }
        return temp;
    }

    Matrix hadamard(Matrix other){
        /*
        elementwise matrix product. 
        returns A elementwise multiplied with B
        */

        if(this->h!=other.h || this->w!=other.w){
            cout<<"matrix dimensions do not match. Calling matrix has size "<<this->h<<"x"<<this->w<<" and the other matrix has size "<<other.h<<"x"<<other.w<<" Hadamard product not possible\n";
            exit(1);
        }
        Matrix temp = Matrix(this->h, this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[other.w*i + j] = this->elements[this->w*i + j]*other.elements[other.w*i + j];
            }
        }
        return temp;
    }

    Matrix power_cpu(int n){
        /*
        each element in the matrix raised to a power n
        returns A^n
        */
        Matrix out = Matrix(this->h, this->w);
        out.init(1.0);
        for(int i=0;i<n;i++){
            for(int i=0;i<this->h;i++){
                for(int j=0;j<this->w;j++){
                    out.elements[this->w*i + j] = out.elements[this->w*i + j]*this->elements[this->w*i + j];
                }
            }            
        }
    
    return out;
    }

    Matrix mat_sqrt(){
        /*
        returns a matrix where each is element is the squareroot of the element at the same position in the calling matrix
        returns A^0.5
        */
        Matrix out = Matrix(this->h, this->w);
        //out.init(1.d);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                out.elements[this->w*i + j] = sqrt(this->elements[this->w*i + j]);
                
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
                means.elements[means.w*0 + j]+=this->elements[this->w*i + j];

            }
        }   

        for(int i=0;i<this->w;i++){
            means.elements[means.w*0 + i] = means.elements[means.w*0 + i]/(double)this->h;
        }
    return means;
    }

    Matrix calc_std(){
        /*
        calculate columnwise standard deviation. 
        Returns (1,this->w) matrix where each element is 
        //the standard deviation of the column/feature that that column represents
        */
        Matrix means = this->calc_means();
        Matrix std = Matrix(1,this->w);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                std.elements[std.w*0 + j]+=pow(this->elements[this->w*i + j] - means.elements[means.w*0 + j],2);
            }
        }
        //cout<<"in here";
        //std = std.power(2);
        std = std.scalar_mul(1.0/(double)this->h);
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
                out.elements[out.w*i + j] = this->elements[this->w*i + j]-means.elements[means.w*0 + j];
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
                out.elements[out.w*i + j] = this->elements[this->w*i + j]/(double)std.elements[std.w*0 + j];
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
                temp.elements[temp.w*i + j] = this->elements[this->w*i + j]+bias.elements[bias.w*0 + j];
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
                this->elements[this->w*i + j] = x;
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
                out+=this->elements[this->w*i + j];
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
                out.elements[out.w*0 + j]+=this->elements[this->w*i + j];
            }
        }
        return out;
    }

    Matrix scalar_mul_cpu(double n){
        /*
        multiply each element by n
        returns n*A
        */
        Matrix temp = Matrix(this->h, this->w);

        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){
                temp.elements[temp.w*i + j] = this->elements[this->w*i + j]*n;
            }
        }
    return temp;
    }

    //used to add the bias as a part of the weights.
    //A columns of ones is prepended to the input
    Matrix add_bias_col(){
        /*
        Now defunct
        */
        Matrix temp = Matrix(this->h,this->w+1);
        for(int i=0;i<temp.h;i++){
            for(int j=0;j<temp.w;j++){
                if(j==0){
                    temp.elements[temp.w*i + j]=1.0;
                }
                else{
                    temp.elements[temp.w*i + j] = this->elements[this->w*i + (j-1)];
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
                double temp = this->elements[this->w*i + j];
                if(temp<m_min){
                    //cout<<"min\n";
                    vec.elements[vec.w*i + j]=m_min;}

                else if(temp>m_max){
                    //cout<<"max\n";
                    vec.elements[vec.w*i + j]=m_max;}
                
                else{
                    //cout<<"where\n";
                    vec.elements[vec.w*i + j]=temp;}
            }
        }
    return vec;
    }

    double* create_array2d(int i, int j){
        /*
        code to create memory for the 2d array
        */
        double *out = new double[i*j];
        return out;
    }

    void print(){
        /*
        pretty prints the matrix
        */
        for(int a=0;a<this->h;a++){
            for(int b=0;b<this->w;b++){
                cout<<this->elements[this->w*a + b]<<" ";

            }
        cout<<endl;
        }
    cout<<endl;
    }

    void rand_init(int s=0){
        /*
        random initialization with numbers from random distribution having variance 2/n
        Weights initialized using He initialization: rand(0,1)*root(2/(w-1)) where w-1 is the size
        of the previous layer
        */
        //srand(time(NULL));
        unsigned seed;
        if(s!=0){seed = s;}
        else{seed = std::chrono::system_clock::now().time_since_epoch().count();}
        std::default_random_engine generator(seed);
        for(int i=0;i<this->h;i++){
            for(int j=0;j<this->w;j++){

                std::uniform_real_distribution<double> distribution(0,1);
                this->elements[this->w*i + j] = distribution(generator)*sqrt(2.0/((double)this->h));
                
                //cout<<this->elements[i][j]<<endl;
            }
        }
    }

    void set_elem(int i, int j, double val){
        /*
        A[i,j]=val
        */
        this->elements[this->w*i + j] = val;
    }

    double get_elem(int i,int j){
        /*
        returns A[i,j]
        */
        return this->elements[this->w*i + j];
    }

    Matrix scalar_mul_gpu(double mul);
    Matrix diff_gpu(Matrix other);
    Matrix power_gpu(double power);
    Matrix transpose_gpu();
    Matrix matmul_gpu(Matrix other);

    Matrix scalar_mul(double mul){
        return this->gpu==1 ? scalar_mul_gpu(mul) : scalar_mul_cpu(mul);
    }

    Matrix diff(Matrix other){
        return this->gpu==1 ? diff_gpu(other) : diff_cpu(other);
    }

    Matrix power(double power){
        return this->gpu==1 ? power_gpu(power) : power_cpu(power);
    }

    Matrix transpose(){
        return this->gpu==1 ? transpose_gpu() : transpose_cpu();
    }

    Matrix matmul(Matrix other){
        return this->gpu==1 ? matmul_gpu(other) : matmul_cpu(other);
    }

};


Matrix Matrix::scalar_mul_gpu(double mul){

    //host output allocation
    Matrix output = Matrix(this->h,this->w);
    //cout<<"inside scalar multiplication"<<endl;

    //device pointers and allocation
    double *d_in, *d_out;
    cudaMalloc(&d_in, this->w*this->h*sizeof(double));
	cudaMalloc(&d_out, this->w*this->h*sizeof(double));
    
    cudaMemcpy(d_in, this->elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_out, output.elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(32,32,1);
    dim3 dimGrid(int(this->h/dimBlock.x)+1,int(this->w/dimBlock.y)+1,1);

    //kernel call
    scalar_mul_kernel<<<dimGrid,dimBlock>>>(d_in, d_out, mul, this->h, this->w);

    cudaMemcpy(output.elements, d_out, this->h*this->w*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_in);
    cudaFree(d_out);

    //output.print();
    return output;

}

Matrix Matrix::diff_gpu(Matrix other){

    //host output allocation
    Matrix output = Matrix(this->h,this->w);

    //device pointers and allocation
    double *d_self, *d_other, *d_out;
    cudaMalloc(&d_self, this->w*this->h*sizeof(double));
	cudaMalloc(&d_other, this->w*this->h*sizeof(double));
    cudaMalloc(&d_out, this->w*this->h*sizeof(double));
    
    cudaMemcpy(d_self, this->elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_other, other.elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_out, output.elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(32,32,1);
    dim3 dimGrid(int(this->h/dimBlock.x)+1,int(this->w/dimBlock.y)+1,1);

    //kernel call
    diff_kernel<<<dimGrid,dimBlock>>>(d_self, d_other, d_out, this->h, this->w);

    cudaMemcpy(output.elements, d_out, this->h*this->w*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_self);
    cudaFree(d_other);
    cudaFree(d_out);

    //output.print();
    return output;

}

Matrix Matrix::power_gpu(double p){

    //host output allocation
    Matrix output = Matrix(this->h,this->w);

    //device pointers and allocation
    double *d_in, *d_out;
    cudaMalloc(&d_in, this->w*this->h*sizeof(double));
	cudaMalloc(&d_out, this->w*this->h*sizeof(double));
    
    cudaMemcpy(d_in, this->elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_out, output.elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(32,32,1);
    dim3 dimGrid(int(this->h/dimBlock.x)+1,int(this->w/dimBlock.y)+1,1);

    //kernel call
    power_kernel<<<dimGrid,dimBlock>>>(d_in, d_out, (double)p, this->h, this->w);

    cudaMemcpy(output.elements, d_out, this->h*this->w*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_in);
    cudaFree(d_out);

    //output.print();
    return output;

}

Matrix Matrix::transpose_gpu(){

    //host output allocation
    Matrix output = Matrix(this->w,this->h);

    //device pointers and allocation
    double *d_in, *d_out;
    cudaMalloc(&d_in, this->w*this->h*sizeof(double));
	cudaMalloc(&d_out, this->w*this->h*sizeof(double));
    
    cudaMemcpy(d_in, this->elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_out, output.elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(32,32,1);
    dim3 dimGrid(int(this->h/dimBlock.x)+1,int(this->w/dimBlock.y)+1,1);

    //kernel call
    transpose_kernel<<<dimGrid,dimBlock>>>(d_in, d_out, this->h, this->w);

    cudaMemcpy(output.elements, d_out, this->h*this->w*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_in);
    cudaFree(d_out);

    //output.print();
    return output;

}

Matrix Matrix::matmul_gpu(Matrix other){

    //host output allocation
    Matrix output = Matrix(this->h,other.w);

    //device pointers and allocation
    double *d_self, *d_other, *d_out;
    cudaMalloc(&d_self, this->w*this->h*sizeof(double));
	  cudaMalloc(&d_other, other.w*other.h*sizeof(double));
    cudaMalloc(&d_out, this->h*other.w*sizeof(double));
    
    cudaMemcpy(d_self, this->elements, this->w*this->h*sizeof(double), cudaMemcpyHostToDevice);
	  cudaMemcpy(d_other, other.elements, other.w*other.h*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_out, output.elements, other.w*this->h*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(32,32,1);
    dim3 dimGrid(int(this->h/dimBlock.x)+1,int(this->w/dimBlock.y)+1,1);

    //the matrices are mxk and kxn so this->h is m, this->w is k and other->w is n
    matmul_kernel<<<dimGrid,dimBlock>>>(d_self, d_other, d_out, this->h, other.w, this->w);

    cudaMemcpy(output.elements, d_out, this->h*other.w*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_self);
    cudaFree(d_other);
    cudaFree(d_out);

    //output.print();
    return output;

}

#endif

