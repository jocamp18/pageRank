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
   double matrix[matrixLength][matrixLength];
   double val;
   int freq = 0;
   srand(time(NULL));
   cout << "Den" << matrixDensity << endl;
   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         double result = (double) rand() / (RAND_MAX);
         if(result < matrixDensity){
            val = (double) rand() / (RAND_MAX);
            val /=10;
            freq ++;
         }else{
            val = 0;
         }
         matrix[i][j] = val;
      }
   }

   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         cout << matrix[j][i] << endl;
      }
   }

   return 0;
}
