//Reference: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//Not really important, used to import the blobs

using namespace std;

std::pair<Matrix,Matrix> read_csv(string name, int num_rows, int num_features){
    
    Matrix input = Matrix(num_rows, num_features);
    Matrix y = Matrix(num_rows,1);
    
    string temp;
    ifstream pima;
    pima.open(name);
    string data;
    int flag = 0;
    int i = 0;
    while(i!=num_rows-1){
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
            //cout<<temp.substr(0, pos);
            val = stod(temp.substr(0, pos));
            //std::cout << val << std::endl;
            temp.erase(0, pos + delimiter.length());

            input.elements[input.w*i + j] = val;
            j++;
        
        }
        //std::cout << i << std::endl;
        y.elements[y.w*i + 0] = stod(temp);
        //std::cout << y.elements[y.w*i + 0] << std::endl;
        i++;
    }

    pima.close();
    //input.print()
    //y.print();

    pair<Matrix, Matrix> result;
    result.first = input;
    result.second = y;
    //cout<<result.second.get_elem(0,0)<<" here"<<endl;

    return result;
}

