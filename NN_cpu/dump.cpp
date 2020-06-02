#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include "model.h"
#include<typeinfo>

using namespace std;


int main(){
    int x=0;
    float arr[1][4][5];
    for(int i=0;i<1;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<5;k++){
                arr[i][j][k] = x++;
                //cout<<arr[i][j][k];

            }
        }
    }
    Matrix t1 = Matrix(10,1);
    Matrix t2 = Matrix(2,3);
    Matrix t3 = Matrix(10,1);
    
    int ctr=1;
    int c2r=1;
    for(int i=0;i<t1.h;i++){
        for(int j=0;j<t1.w;j++){
            t1.elements[i][j]=ctr++;
            //t2.elements[i][j]=5;

        }
    }
    for(int i=0;i<t3.h;i++){
        for(int j=0;j<t3.w;j++){
            t3.elements[i][j]=c2r++;

        }
    }
    Matrix t4(400,1);
    t4.init(4.d);
    t4 = t4.scalar_mul(1.d);
    //t4 = t4.power(2);
    Matrix t5 = Matrix(400,1);
    t5.init(5.d);
    //t1.diff(t3);
    //(t4.diff(t5)).print();
    //Matrix s = t1.matmul(t3);
    //t1.print();
    //Matrix s = t1.hadamard(t3);
    //double red = t2.sum_reduce();
    //cout<<"reduce sum: "<<red<<endl;
    t3.init(0);
    //t4.print();
    //print_matrix(t1);
    //print_matrix(t3);
    //print_matrix(s);


    Linear *W1,*W2,*W3;
    Linear Lx=Linear(1,1,"W1",1);
    Linear Ly=Linear(5,4,"W2",0);
    Linear Lz=Linear(1,5,"W3",0);

    W1 = &Lx;
    W2 = &Ly;
    W3 = &Lz;

    ReLU *r1,*r2,*r3;
    ReLU lx=ReLU("Default ReLU");
    ReLU ly = ReLU("D2R2");
    ReLU lz = ReLU("D3R3");

    r1 = &lx;
    r2 = &ly;
    r3 = &lz;
    
    Sigmoid *s1;
    Sigmoid ls = Sigmoid("Only sigmoid");
    s1 = &ls;
    
    //(y.W).print();
    Model m = Model();
    m.add(W1, linear);
    //m.add(r1, relu);
    //m.add(W2, linear);
    //m.add(r2, relu);
    //m.add(W3, linear);
    //m.add(r3, relu);
    m.add(s1,sigmoid);
    //cout<<typeid(W4).name();
    m.print_model();
    int temp;cin>>temp;
    MSE_Loss L1 = MSE_Loss();
    m.train(t1,t3,0.001,10000,L1, 1);

    
    
    return 0;
}

