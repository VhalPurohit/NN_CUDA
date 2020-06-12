#include<iostream>
#include "matrix.h"
//contains all the GPU kernels

//scalar multiplication kernel
__global__ void scalar_mul_kernel(double arr[],double out[], double mul,int M, int N){
    int row = threadIdx.x + blockIdx.x*blockDim.x;
    int col = threadIdx.y + blockIdx.y*blockDim.y;
    if(row<M && col<N){
        out[row*N + col] = mul*arr[row*N + col];
        //printf("%d",arr[row*N + col]);
    }
}

//difference between matrices kernel
__global__ void diff_kernel(double arr_self[], double arr_other[], double out[], int M, int N){
    int row = threadIdx.x + blockIdx.x*blockDim.x;
    int col = threadIdx.y + blockIdx.y*blockDim.y;
    if(row<M && col<N){
        out[row*N + col] = arr_self[row*N + col] - arr_other[row*N + col];
        //printf("%d",arr_self[row*N + col]);
    }
}

//matrix raised to a power
__global__ void power_kernel(double arr[],double out[], double p,int M, int N){
    int row = threadIdx.x + blockIdx.x*blockDim.x;
    int col = threadIdx.y + blockIdx.y*blockDim.y;
    if(row<M && col<N){
        out[row*N + col] = pow(arr[row*N + col],p);
        //printf("%d",arr[row*N + col]);
    }
}

//transpose
__global__ void transpose_kernel(double arr[],double out[], int M, int N){
    int row = threadIdx.x + blockIdx.x*blockDim.x;
    int col = threadIdx.y + blockIdx.y*blockDim.y;
    if(row<M && col<N){
        out[M*col + row] = arr[N*row + col];
        //printf("%d",arr[row*N + col]);
    }
}

//matmul
__global__ void matmul_kernel(double arr_self[], double arr_other[], double out[], int M, int N, int mid){
    int row = threadIdx.x + blockIdx.x*blockDim.x;
    int col = threadIdx.y + blockIdx.y*blockDim.y;
    if(row<M && col<N){
        double temp = 0.0;
        for(int k=0;k<mid;k++){
            temp += arr_self[row*mid + k]*arr_other[k*N + col]; 
        }
        out[row*N + col] = temp;
        //printf("%d",arr_self[row*N + col]);
    }
}

