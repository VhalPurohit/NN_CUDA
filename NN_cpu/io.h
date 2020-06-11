<<<<<<< HEAD
//Reference: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//Not really important, used to import the pima indians dataset

using namespace std;

std::pair<Matrix,Matrix> read_csv(string name){
    
    Matrix input = Matrix(768, 8);
    Matrix y = Matrix(768,1);
    
    string temp;
    ifstream pima;
    pima.open(name);
    string data;
    int flag = 0;
    int i = 0;
    while(!pima.eof()){
        if(flag==0){
            getline(pima,temp);
            flag=1;
            continue;
        }
        getline(pima,temp);
        
        std::string delimiter = ",";

        size_t pos = 0;
        int j=0;
        while ((pos = temp.find(delimiter)) != std::string::npos) {
            
            double val;
            val = stod(temp.substr(0, pos));
            //std::cout << val << std::endl;
            temp.erase(0, pos + delimiter.length());

            input.elements[i][j] = val;
            j++;
        
        }
        
        y.elements[i][0] = stod(temp);
        //std::cout << temp << std::endl;
        i++;
    }

    pima.close();
    //input.print()
    //y.print();

    pair<Matrix, Matrix> result;
    result.first = input;
    result.second = y;

    return result;
}

=======
//Reference: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//Not really important, used to import the pima indians dataset

using namespace std;

std::pair<Matrix,Matrix> read_csv(string name){
    
    Matrix input = Matrix(768, 8);
    Matrix y = Matrix(768,1);
    
    string temp;
    ifstream pima;
    pima.open(name);
    string data;
    int flag = 0;
    int i = 0;
    while(!pima.eof()){
        if(flag==0){
            getline(pima,temp);
            flag=1;
            continue;
        }
        getline(pima,temp);
        
        std::string delimiter = ",";

        size_t pos = 0;
        int j=0;
        while ((pos = temp.find(delimiter)) != std::string::npos) {
            
            double val;
            val = stod(temp.substr(0, pos));
            //std::cout << val << std::endl;
            temp.erase(0, pos + delimiter.length());

            input.elements[i][j] = val;
            j++;
        
        }
        
        y.elements[i][0] = stod(temp);
        //std::cout << temp << std::endl;
        i++;
    }

    pima.close();
    //input.print()
    //y.print();

    pair<Matrix, Matrix> result;
    result.first = input;
    result.second = y;

    return result;
}

>>>>>>> f7fe4ea55f019ad04e12d24eebf8bc3effbb4408
