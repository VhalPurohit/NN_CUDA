#include<iostream>
#include "layers.h"
#include "matrix.h"
#include "loss.h"
#include<variant>
#include<any>
#include<typeinfo>
#include<cmath>
#include<stdio.h>
using namespace std;

class Model{
    public:
    //vector<Object> layers;
    //vector<variant<Linear, ReLU>> layers;
    LAYER l_lin = linear, l_relu = relu, l_sigmoid = sigmoid;
    vector<LAYER> l_t;
    vector<Layer*> layers;
    
       
    Model(){}

    template <typename T>
    void add(T l, LAYER t){
        layers.push_back(l);
        l_t.push_back(t);
    }

    //template <typename T>
    void print_model(){
        cout<<"The model has the following layers: \n";
        for(int i=0;i<layers.size();i++){
            
            if(this->l_t[i]==l_lin){
                Linear *temp = dynamic_cast<Linear*>(layers[i]);
                cout<<temp->taip<<" named "<<temp->name<<endl;
    
            }
            else if(this->l_t[i]==l_relu){
                ReLU *temp = dynamic_cast<ReLU*>(layers[i]);
                cout<<temp->taip<<" named "<<temp->name<<endl;
            }
            
            else if(this->l_t[i]==l_sigmoid){
                Sigmoid *temp = dynamic_cast<Sigmoid*>(layers[i]);
                cout<<temp->taip<<" named "<<temp->name<<endl;
            }
            //cout<<"The model has the following layers: \n"<<layers[i].taip<<" named "<<layers[i].name<<endl;
            //cout<<"The model has the following layers: \n"<<temp->taip<<" named "<<temp->name<<endl;
            
        }
    }

    Matrix forward(Matrix X){
        if(this->layers.size()==0){
            cout<<"The model is empty, please add layers"<<endl;
            exit(1);
        }
        //do enum
        //-1 is a placeholder
        //Matrix placeholder = Matrix(X.h,-1);
        Matrix temp = X;
        for(int i=0;i<this->layers.size();i++){

            //Matrix temp;
            if(this->l_t[i]==l_lin){
                Linear *l_p = dynamic_cast<Linear*>(layers[i]);
                //cout<<l_p->name<<endl;
                temp = l_p->forward(temp);
            }
            else if(this->l_t[i]==l_relu){
                ReLU *l_p = dynamic_cast<ReLU*>(layers[i]);
                //cout<<l_p->name<<endl;
                temp = l_p->forward(temp);
            }
            else if(this->l_t[i]==l_sigmoid){
                Sigmoid *l_p = dynamic_cast<Sigmoid*>(layers[i]);
                //cout<<l_p->name<<endl;
                temp = l_p->forward(temp);
            }
        }
        return temp;
    }

    Matrix backward(Matrix dY){
        
        //-1 is a placeholder
        //Matrix placeholder = Matrix(X.h,-1);
        Matrix temp = dY;
        for(int i=this->layers.size()-1;i>=0;i--){
            //Matrix temp;
            //cout<<i<<endl;
            if(this->l_t[i]==l_lin){
                Linear *l_p = dynamic_cast<Linear*>(layers[i]);
                //cout<<l_p->name;
                temp = l_p->backward(temp);
            }
            else if(this->l_t[i]==l_relu){
                ReLU *l_p = dynamic_cast<ReLU*>(layers[i]);
                temp = l_p->backward(temp);
            }
            else if(this->l_t[i]==l_sigmoid){
                Sigmoid *l_p = dynamic_cast<Sigmoid*>(layers[i]);
                temp = l_p->backward(temp);
            }
        }
        return temp;
    }

    void step(float alpha){
        for(int i=this->layers.size()-1;i>=0;i--){
            if(this->l_t[i]==l_lin){
                Linear *l_p = dynamic_cast<Linear*>(layers[i]);
                l_p->step(alpha);
            }
        }
    }

    float round_5(double x){
            return (float)ceil(x*pow(10,5))/100000.f;
        }
    
    Matrix get_categorical_matrix(Matrix Y_hat){
        Matrix out = Matrix(Y_hat.h, Y_hat.w);
        for(int i=0;i<Y_hat.h;i++){
            for(int j=0;j<Y_hat.w;j++){
                if(Y_hat.elements[i][j]<0.5){
                    out.elements[i][j] = 0;
                }
                else{out.elements[i][j]=1;}
            }
        }
        return out;
    }

    double get_accuracy(Matrix Y_hat, Matrix Y){
        
        
        int match = 0;
        Y_hat = get_categorical_matrix(Y_hat);
        for(int i=0;i<Y.h;i++){
            for(int j=0;j<Y.w;j++){
                if(int(Y_hat.elements[i][j])==int(Y.elements[i][j])){
                    match++;
                }
            }
        }
        return ((double)match)/((double)Y_hat.h * Y_hat.w);
    }

    vector<double> train(Matrix X, Matrix y, float alpha, int epochs, MSE_Loss L1, int categorical=1){
        vector<double> loss_tab;

        for(int i=0;i<epochs;i++){
            
            Matrix y_hat = this->forward(X);

            Matrix loss_matrix = L1.compute(y_hat,y);
            double loss = loss_matrix.sum_reduce();
            cout<<"Loss at epoch "<<i<<" : "<<loss<<endl;
            loss_tab.push_back(loss);
            if(categorical==1){
                double accuracy = get_accuracy(y_hat, y);
                cout<<"accuracy is: "<<accuracy*100<<"%"<<endl;
                }
            Matrix dY = L1.backward();

            this->backward(dY);

            this->step(alpha);



        }

        return loss_tab;
    }

};