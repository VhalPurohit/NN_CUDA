<<<<<<< HEAD
#include<iostream>
#ifndef UTILS_H
#define UTILS_H

using namespace std;

float round_5(double x){
        /*
        round the number to 5 places after the decimal
        */
        return (float)ceil(x*pow(10,5))/100000.f;
    }

Matrix get_categorical_matrix(Matrix Y_hat){
    /*
    convert the matrix to categorical matrix of 1 and 0, used to calculate classification accuracy
    */
    Matrix out = Matrix(Y_hat.h, Y_hat.w);
    for(int i=0;i<Y_hat.h;i++){
        for(int j=0;j<Y_hat.w;j++){
            double temp = Y_hat.elements[i][j];
            if(temp>1 || temp<0){
                cout<<"Probabilities need to be nbetween 1 and 0.\n";
                exit(1);
            }
            
            if(Y_hat.elements[i][j]<0.5){
                out.elements[i][j] = 0;
            }
            else{out.elements[i][j]=1;}
        }
    }
    return out;
}

double get_accuracy(Matrix Y_hat, Matrix Y){
    /*
    used to calculate the accuracy of prediction

    Y_hat:the predictions
    Y: the ground truth
    */
    int match = 0;
    
    Y_hat = get_categorical_matrix(Y_hat);
    //Y_hat.print();
    for(int i=0;i<Y.h;i++){

        int flag = 0;
        for(int j=0;j<Y.w;j++){
            if(int(Y_hat.elements[i][j])!=int(Y.elements[i][j])){
                flag = 1;
                break;   
            }
        }
        if(flag==0){
            match++;
        }
    }
    return ((double)match)/((double)Y_hat.h * Y_hat.w);
}

int check_ground_truth(Matrix Y_hat){
    /*
    check that the ground truth is categorical
    */
    for(int a=0;a<Y_hat.h;a++){
        for(int b=0;b<Y_hat.w;b++){
            if(Y_hat.elements[a][b]!=0 && Y_hat.elements[a][b]!=1){
                cout<<"Something's wrong with your data"<<endl;
                return 1;
            }
        }
    }
    return 0;
}
=======
#include<iostream>
#ifndef UTILS_H
#define UTILS_H

using namespace std;

float round_5(double x){
        /*
        round the number to 5 places after the decimal
        */
        return (float)ceil(x*pow(10,5))/100000.f;
    }

Matrix get_categorical_matrix(Matrix Y_hat){
    /*
    convert the matrix to categorical matrix of 1 and 0, used to calculate classification accuracy
    */
    Matrix out = Matrix(Y_hat.h, Y_hat.w);
    for(int i=0;i<Y_hat.h;i++){
        for(int j=0;j<Y_hat.w;j++){
            double temp = Y_hat.elements[i][j];
            if(temp>1 || temp<0){
                cout<<"Probabilities need to be nbetween 1 and 0.\n";
                exit(1);
            }
            
            if(Y_hat.elements[i][j]<0.5){
                out.elements[i][j] = 0;
            }
            else{out.elements[i][j]=1;}
        }
    }
    return out;
}

double get_accuracy(Matrix Y_hat, Matrix Y){
    /*
    used to calculate the accuracy of prediction

    Y_hat:the predictions
    Y: the ground truth
    */
    int match = 0;
    
    Y_hat = get_categorical_matrix(Y_hat);
    //Y_hat.print();
    for(int i=0;i<Y.h;i++){

        int flag = 0;
        for(int j=0;j<Y.w;j++){
            if(int(Y_hat.elements[i][j])!=int(Y.elements[i][j])){
                flag = 1;
                break;   
            }
        }
        if(flag==0){
            match++;
        }
    }
    return ((double)match)/((double)Y_hat.h * Y_hat.w);
}

int check_ground_truth(Matrix Y_hat){
    /*
    check that the ground truth is categorical
    */
    for(int a=0;a<Y_hat.h;a++){
        for(int b=0;b<Y_hat.w;b++){
            if(Y_hat.elements[a][b]!=0 && Y_hat.elements[a][b]!=1){
                cout<<"Something's wrong with your data"<<endl;
                return 1;
            }
        }
    }
    return 0;
}
>>>>>>> f7fe4ea55f019ad04e12d24eebf8bc3effbb4408
#endif