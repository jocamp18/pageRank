#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <random>
#include <vector>
#include <map>
#include <stdio.h>
#include <algorithm> 
#include <string>
using namespace std;
vector<float> values;
vector<int> columns;
vector<int> pointerB;
vector<int> pointerE;
vector<double> r;
vector<double> auxR;
vector<double> sub;
vector<double> auxiliarR;
vector<double> substract(int matrixLength){
   for(int i = 0; i < matrixLength; i++){
      sub[i] = abs(r[i] - auxiliarR[i]);
      //cout << "result " << sub[i] << endl;
   }
   return sub;
}

vector<double> multiply(int matrixLength){
   for(int i = 0; i < matrixLength; i++){
      auxR[i] = 0.0;
   }
    for (int i = 0; i < matrixLength; i++) {
      for (int j = pointerB[i]; j < pointerE[i]; j++) {
         auxR[i] += (values[j] * r[columns[j]]);
      }
      //cout << "m: " << auxR[i] << endl;
   }
   return auxR;
}

int main(int argc, char **argv){
   int matrixLength = atoi(argv[1]);
   double matrixDensity = atof(argv[2]);
   double tolerance = atof(argv[3]);
   //double matrix[3][3] = {{0,0.5,1},{0.3,0,0},{0.7,0.5}};
   double matrix[matrixLength][matrixLength];
   double auxArray[matrixLength];
   double val;
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
  /* 
   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         cout << matrix[i][j] << " | ";
      }
      cout << endl;
   }
   */
   int pos = 0, cont = 0;
   for(int i = 0; i < matrixLength; i++){
      pointerB.push_back(pos);
      for(int j = 0; j < matrixLength; j++){
         if(matrix[i][j] != 0){
            cont++;
            values.push_back(matrix[i][j]);
            columns.push_back(j);
            pos ++;
         }
      }
      pointerE.push_back(pos);
   }
   
   double probability = (double)(1.0 / (double)matrixLength);
   for (int i = 0; i < matrixLength; i++) {
      r.push_back(probability);
      auxR.push_back(0.0);
      sub.push_back(0.0);
   }
   vector<double>::iterator position;
   do{
      auxiliarR = r;
      r = multiply(matrixLength);
      sub = substract(matrixLength);
      position = max_element(sub.begin(), sub.end());
      cout << "Tolerance: "<< tolerance << " Maximum: " << *position << endl;
   }while(tolerance < *position);
   cout << "r vector: " << endl;
   for(double n: r){
      cout << n << endl;
   }
  /* 
   int cont1 = 0;
   for(int i = 0; i < pointerB.size();i++){
      int min = pointerB[i];
      int max = pointerE[i];
      for(int j = min; j < max;j++){
         cout << "cont " << cont1 << " send " << values[j] << endl;
      }
      cont1++;
   }*/
  /* 
   cout << "values: ";
   for (double n : values){
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
   
   cout << "cont " << cont << endl;*/
   return 0;
}
