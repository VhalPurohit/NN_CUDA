#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include "model.h"
#include "io.h"
#include<typeinfo>
#include "utils.h"
#include<chrono>
using namespace std;
//you can run this using 
//g++ dump.cu -o output.exe
//./output.exe

//to use gpu: go to matrix.h and set the attribute gpu=1



int main(){
    //something is wrong with the io function so if gpu is used please uncomment the lines marked by 
    //this sign ->
    string filename = "data.csv";
    pair<Matrix, Matrix> PAIR = read_csv(filename, 1000, 2);
    Matrix input = PAIR.first;
    Matrix y = PAIR.second;
    cout<<y.h<<" "<<y.w<<" "<<endl;
    
    //->int iii = 1000;
    //->y.set_elem(iii-1,0,0);
    //->input.set_elem(iii-1,0,0);
    //->input.set_elem(iii-1,1,0);
    
    //y.print();
   // input.print();
    
    //means normalize the input to get converge
    input = input.ms_normalise();
    
    cout<<"\nthe std of the input is(should be 1 for each column since it is normalized)\n";
    input.calc_std().print();

    cout<<"\nthe mean of each feature is(should be 0 for each column since it is normalized)\n";
    input.calc_means().print();

    int c_temp;
    cout<<"Press any key and enter to continue"<<endl;
    cin>>c_temp;
    
    //like I said, something's wrong with the IO, when GPU is used, please uncomment those lines 
    //or the last value is somehow garbage (they set the last values for x and y to be 0)
    int flag = check_ground_truth(y);
    if(flag==1){
        cout<<"Ground truth failed the check";
        return 1;
    }

    //Hyper-parameters
    int epochs = 1000;
    float lr = 0.1;

    //Layer init
    Linear *W1,*W2,*W3;
    W1 = new Linear(20,input.w,"W1",1);
    W2 = new Linear(30,20,"W2",1);
    W3 = new Linear(1,30,"W3",1);
    
    ReLU *r1,*r2;
    r1 = new ReLU("Default ReLU");
    r2 = new ReLU("D2R2");
    
    Sigmoid *s1;
    s1 = new Sigmoid("Only sigmoid");
    //s2 = new Sigmoid("Only sigmoid");

    Model m = Model();
    m.add(W1);
    m.add(r1);
    m.add(W2);
    m.add(r2);
    m.add(W3);
    m.add(s1);
    
    MSE_Loss *L1 = new MSE_Loss();

    pair<vector<double>,vector<double>> pair;

    auto start = std::chrono::high_resolution_clock::now(); 

    pair = m.train(input,y,lr,epochs,L1, 1, 1);

    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start); 

    std::cout<<"Time taken in milliseconds for the model to train is: "<<duration.count()*0.000001<<endl;

    cout<<"loss values are"<<endl;
    vector<double> loss_vals = pair.first;
    vector<double> acc_vals = pair.second;
    for(int i = 0;i<loss_vals.size();i++){
    cout<<loss_vals[i]<<", ";
    }
    cout<<"\nacc vals are \n";
    for(int i=0;i<acc_vals.size();i++){
    cout<<acc_vals[i]<<", ";
    }
    
    return 0;
}
