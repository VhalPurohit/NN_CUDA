#include<iostream>
//#include "layers.h"
#include "matrix.h"
//#include "loss.h"
//#include "model.h"
//#include "io.h"
//#include<typeinfo>
//#include "utils.h"
using namespace std;


int main(){
    Matrix A = Matrix(2,2);
    Matrix B = Matrix(10000,500);
    Matrix C = Matrix(5,8);
    //A.init(1);
    //A.print();
    //B.init(-5);
    //B.print();
    int ctr1 = 0;
    int ctr2 = 0;
    for(int i=0;i<A.h;i++){
        for(int j=0;j<A.w;j++){
            A.set_elem(i,j,++ctr1);
        }
    }

    for(int i=0;i<B.w;i++){
        for(int j=0;j<B.h;j++){
            B.set_elem(j,i,++ctr2);
        }
    }
    //C.print();
    //B.init(4);
    //B.print();
    B.set_elem(0,0,111);
    cout<<B.get_elem(0,0);
    return 0;
}