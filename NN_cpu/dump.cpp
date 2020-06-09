#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include "model.h"
#include "io.h"
#include<typeinfo>
#include "utils.h"
using namespace std;




int main(){
    string filename = "diabetes.csv";
    pair<Matrix, Matrix> PAIR = read_csv(filename);
    Matrix input = PAIR.first;
    Matrix y = PAIR.second;
    //cout<<y.h<<" "<<y.w<<" "<<endl;
    input = input.ms_normalise();

    int flag = check_ground_truth(y);
    if(flag==1){
        cout<<"Ground truth failed the check";
        return 1;
    }


    /*
    Matrix t1 = Matrix(1000,8);
    //Matrix t2 = Matrix(2,3);
    Matrix t3 = Matrix(10,10);
    Matrix ttt = Matrix(1,10);
    ttt.init(1.d);
    t1.rand_init(2);
    t1 = t1.ms_normalise();

    for(int i=0;i<y.h;i++){
        for(int j=0;j<y.w;j++){
            y.elements[i][j] = double(y.elements[i][j]);
        }
    }
    //y.print();
    //t3.rand_init(0);

    //t3.print();
    int ctr=1;
    int c2r=1;
    for(int i=0;i<t3.h;i++){
        for(int j=0;j<t3.w;j++){
           t3.elements[i][j]=(double)ctr++;
            //t1.elements[i][j]=ctr++;
            //t2.elements[i][j]=5;

        }
    }
    //t3 = t3.bias_add(ttt);
    //t3.print();
    //t3.clip(10,19).print();
    //t3.reduce_axis_0().print();
    //t3 = t3.ms_normalise();
    //t3.calc_std().print();
    //t3.print();
    //Matrix m2 = t3.calc_means();
    //m2.print();
    //for(int i=0;i<t3.h;i++){
    //    t3.elements[i][0]=0.d;
    //    t3.elements[i][1]=1.d;
    //
    //}
    //t1.print();
    //cout<<"ma";
    //Matrix t4(400,1);
    //t4.init(4.d);
    //t4 = t4.scalar_mul(1.d);
    //t4 = t4.power(2);
    //Matrix t5 = Matrix(400,1);
    //t5.init(5.d);
    //t1.diff(t3);
    //(t4.diff(t5)).print();
    //Matrix s = t1.matmul(t3);
    //t1.print();
    
    //t3.init(0);
    
    //Matrix s = t1.hadamard(t3);
    //double red = t2.sum_reduce();
    //cout<<"reduce sum: "<<red<<endl;

    //t4.print();
    //print_matrix(t1);
    //print_matrix(t3);
    //print_matrix(s);

    */

    Linear *W1,*W2,*W3;
    W1 = new Linear(1024,input.w,"W1",1);
    W2 = new Linear(12,1024,"W2",1);
    W3 = new Linear(1,12,"W3",1);

    ReLU *r1,*r2,*r3;
    ReLU lx=ReLU("Default ReLU");
    ReLU ly = ReLU("D2R2");
    ReLU lz = ReLU("D3R3");

    r1 = new ReLU("Default ReLU");
    r2 = new ReLU("D2R2");
    r3 = new ReLU("D3R3");
    
    Sigmoid *s1, *s2;
    s1 = new Sigmoid("Only sigmoid");
    s2 = new Sigmoid("Only sigmoid");

    Model m = Model();
    m.add(W1);
    m.add(r1);
    m.add(W2);
    m.add(r2);
    m.add(W3);
    //m.add(r3);
    m.add(s1);
    
    MSE_Loss *L1 = new MSE_Loss();

    //m.train(t1,t1,1000,7,L1, 1);
    m.train(input,y,0.0001,100000,L1, 1, 1);
    
    return 0;
}
