#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <random>
#include <vector>
#include <map>
#include <stdio.h>
using namespace std;
int main(int argc, char **argv){
   int matrixLength = atoi(argv[1]);
   double matrixDensity = atof(argv[2]);
   //int matrix[5][5] = {{1,-1,0,-3,0},{-2,5,0,0,0},{0,0,4,6,4},{-4,0,2,7,0},{0,8,0,0,-5}};
   double matrix[matrixLength][matrixLength];
   double auxArray[matrixLength];
   double val;
   vector<float> values(matrixLength);
   vector<int> columns;
   vector<int> pointerB;
   vector<int> pointerE;
   srand(time(NULL));
   double  sum;
   for(int i = 0; i < matrixLength; i++){
      sum = 0;
      for(int j = 0; j < matrixLength; j++){
         double result = (double) rand() / (RAND_MAX);
         if(i == j){
            val = 0;
         }else if(result < matrixDensity){
            val = (double) rand() / (RAND_MAX);
         }else{
            val = 0;
         }
         sum += val;
         matrix[j][i] = val;
      }
      //cout << sum << endl;
      auxArray[i] = sum;
   }
   
   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         if(matrix[j][i] != 0){
            matrix[j][i] = matrix[j][i]/auxArray[i];
         }
      }
   }
   
   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         cout << matrix[i][j] << " | ";
      }
      cout << endl;
   }
   int pos = 0;
   for(int i = 0; i < matrixLength; i++){
      pointerB.push_back(pos);
      for(int j = 0; j < matrixLength; j++){
         if(matrix[i][j] != 0){
            values.push_back(matrix[i][j]);
            columns.push_back(j);
            pos ++;
         }
      }
      pointerE.push_back(pos);
   }
   
   cout << "values: ";
   for (int n : values){
      cout << n << ' ';
   }
   cout << "columns: ";
   for (int n : columns){
      cout << n << ' ';
   }
   cout << "pointerB: ";
   for (int n : pointerB){
      cout << n << ' ';
   }
   
   cout << "pointerB: ";
   for (int n : pointerE){
      cout << n << ' ';
   }
   cout << "" << endl;
   return 0;
}
