# NN_CUDA
 Deep Learning framework using C++ CUDA
 Currently only contains the C++ impementation. This is how it works:
 
 The following layers are defined: Linear, ReLU, Sigmoid
 The following losses are defined: MSE
 
 Create pointers of type layer with the right parameters, 
 then create a model object, adding the layers to the model using model.add()
 
 All operations are done on a data structure called Matrix. Source code provided
 To run the code, use these commands called from the directory where dump.cpp is stored (all the files need to be in the same directory)
 
 g++ -o output.exe dump.cpp
 
 ./output.exe
