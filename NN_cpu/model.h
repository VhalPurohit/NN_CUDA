#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include "utils.h"
#include<variant>
#include<any>
#include<typeinfo>
#include<cmath>
#include<memory>
#include<stdio.h>
using namespace std;

class Model{
    public:
    
    vector<Layer*> layers;
    
       
    Model(){}

    template <typename T>
    void add(T l){
        layers.push_back(l);
    }

    //template <typename T>
    void print_model(){
        /*
        Loops over the layers and prints their name
        */
        cout<<"The model has the following layers: \n";

        for(int i=0;i<layers.size();i++){
        
            cout<<layers[i]->get_taip()<<" named "<<layers[i] ->get_name()<<endl;
        
        }
    }

    Matrix forward(Matrix& X){
        /*
        loops over the layers and calls forward on each of them
        */
        if(this->layers.size()==0){
            cout<<"The model is empty, please add layers"<<endl;
            exit(1);
        }

        Matrix temp = X;
        for(int i=0;i<this->layers.size();i++){
            //temp = temp.clip(-2,2);
            temp = layers[i]->forward(temp);
        }
        return temp;
    }

    Matrix backward(Matrix& dY){
        
        Matrix temp = dY;
        for(int i=this->layers.size()-1;i>=0;i--){
            //temp = temp.clip(-5,5);
            //temp.print();
            temp = layers[i]->backward(temp);
        }
        return temp;
    }

    void step(float alpha){
        for(int i=this->layers.size()-1;i>=0;i--){
            layers[i]->step(alpha);
        }
    }

    vector<double> train(Matrix &X, Matrix &y, float alpha, int epochs, Loss *L1, int categorical=1, int verbose=1){
        vector<double> loss_tab;

        for(int i=0;i<epochs;i++){
            
            Matrix y_hat = this->forward(X);
            //y_hat.print();

            Matrix loss_matrix = L1->compute(y_hat,y);
            
            double loss = loss_matrix.sum_reduce();

            if(verbose==1 || i==(epochs-1)){
                cout<<"Loss at epoch "<<i<<" : "<<loss<<endl;
                //loss_tab.push_back(loss);
                if(categorical==1){
                    double accuracy = get_accuracy(y_hat, y);
                    cout<<"accuracy is: "<<accuracy*100<<"%"<<endl;
                    }
                }
            Matrix dY = L1->backward();

            this->backward(dY);
            
            this->step(alpha);



        }

        return loss_tab;
    }

};