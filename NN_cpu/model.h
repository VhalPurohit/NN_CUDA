#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include "utils.h"
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
        /*
        This function takes in pointers to an object of any child of Layer and stores it onto the vector
        */
        layers.push_back(l);
    }

    //template <typename T>
    void print_model(){
        /*
        Loops over the layers and prints their name and type
        */
        cout<<"The model has the following layers: \n";

        for(int i=0;i<layers.size();i++){
        
            cout<<layers[i]->get_taip()<<" named "<<layers[i] ->get_name()<<endl;
        
        }
    }

    Matrix forward(Matrix& X){
        /*
        loops over the layers and calls forward on each of them

        input
        X: the features of the dataset

        returns
        temp: teh prediction of the dataset
        */
        if(this->layers.size()==0){
            cout<<"The model is empty, please add layers"<<endl;
            exit(1);
        }

        Matrix temp = X;
        for(int i=0;i<this->layers.size();i++){
            //temp = temp.clip(-2,2);
            //temp.print();
            temp = layers[i]->forward(temp);
        }
        return temp;
    }

    Matrix backward(Matrix& dY){
        /*
        Computes the backward pass over the model

        inputs
        dY: The gradient of the loss wrt the prediction of the model

        returns
        temp: the gradient of the input, please don't pay attention to it, its not even stored
        */
        Matrix temp = dY;
        for(int i=this->layers.size()-1;i>=0;i--){
            //temp = temp.clip(-5,5);
            //temp.print();
            temp = layers[i]->backward(temp);
        }
        return temp;
    }

    void step(float alpha){
        /*
        Performs weight update step

        inputs
        alpha: the learning rate for gradient descent
        */
        for(int i=this->layers.size()-1;i>=0;i--){
            layers[i]->step(alpha);
        }
    }

    pair<vector<double>,vector<double>> train(Matrix &X, Matrix &y, float alpha, int epochs, Loss *L1, int categorical=1, int verbose=1){
        /*
        Trains the model

        inputs
        X: the input features
        y: the ground truth
        alpha: the learning rate
        epochs: the number of epochs to iterate over
        *L1: pointer to the loss. It is a pointer so that one can have user defined losses too if one wants
        categorical: 1 if it is a classification problem and the ground truth is categorical, accuracy is 
        calculated in this case. Otherwise only the loss is calculated
        verbose: 1 if loss needs to be printed else 0

        returns:
        pair<vector,vector>: a vector containing the losses, a vector containing the accuracies
        */
        
        vector<double> loss_tab;
        vector<double> acc_tab;

        for(int i=0;i<epochs;i++){
            
            Matrix y_hat = this->forward(X);
            //y_hat.print();

            Matrix loss_matrix = L1->compute(y_hat,y);
            
            double loss = loss_matrix.sum_reduce();

            if(verbose==1 || i==(epochs-1)){
                cout<<"Loss at epoch "<<i<<" : "<<loss<<endl;
                //only track the loss every 400 epochs since vanilla gradient descent takes a long time to converge
                if(i%40==0){
                    loss_tab.push_back(loss);}
                if(categorical==1){
                    double accuracy = get_accuracy(y_hat, y);
                    cout<<"accuracy is: "<<accuracy*100<<"%"<<endl;
                    if(i%40==0){
			acc_tab.push_back(accuracy*100);
			}
                    }
                }
            Matrix dY = L1->backward();

            this->backward(dY);
            
            this->step(alpha);



        }
        //cout<<"loss_tab "<<loss_tab.size()<<endl;
        pair<vector<double>, vector<double>> PAIR(loss_tab,acc_tab);
        return PAIR;
    }

};
