#include <mpi.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <algorithm> 
#include <string>
#include <unistd.h>
#include <time.h>

using namespace std;
vector<int> myIntVector;
int world_size, world_rank;
char processor_name[MPI_MAX_PROCESSOR_NAME];
int name_len;
int matrixLength;
double tolerance;
double matrixDensity;

vector<double> values;
vector<int> columns;
vector<int> pointerB;
vector<int> pointerE;

vector<double> r;
vector<double> auxiliarRMPI;
vector<double> auxiliarR;
vector<double> sub;

void initMPI(int argc, char** argv){
   // Initialize the MPI environment
   MPI_Init(NULL, NULL);
   // Get the number of processes
   MPI_Comm_size(MPI_COMM_WORLD, &world_size);
   // Get the rank of the process
   MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
   // Get the name of the processor
   MPI_Get_processor_name(processor_name, &name_len);
}

int initial(int id){
   double res = (double) matrixLength/(world_size - 1);
   return ceil(res) * (id - 1);
}

int final(int id){
   double res = (double) matrixLength/(world_size - 1);
   int finalPosition = ceil(res) * id;
   if(finalPosition < matrixLength){
      return finalPosition;
   }else{
      return matrixLength;
   }
}

vector<double> multiply(int initial, int final, int id){
   vector<double> auxR;
   for(int i = 0; i < matrixLength; i++){
      auxR.push_back(0.0);
   }
   for (int i = initial; i < final; i++) {
      for (int j = pointerB[i]; j < pointerE[i]; j++) {
         auxR[i] += (values[j] * r[columns[j]]);
      }
   }
   return auxR;
}

double** createMatrix(){
   double** matrix;
   matrix = new double*[matrixLength];
   
   for(int i = 0; i < matrixLength; i++){
      matrix[i] = new double[matrixLength];
   }
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
      for(int j = 0; j < matrixLength; j++){
         if(matrix[j][i] != 0){
            matrix[j][i] = matrix[j][i]/auxArray[i];
         }
      }
   }
   return matrix;
}

void printMatrix(double **matrix){
   for(int i = 0; i < matrixLength; i++){
      for(int j = 0; j < matrixLength; j++){
         cout << matrix[i][j] << " | ";
      }
      cout << endl;
   }
}

void initializeR(){
   double probability = (double)(1.0 / (double)matrixLength);
   for (int i = 0; i < matrixLength; i++) {
      r.push_back(probability);
      auxiliarRMPI.push_back(0.0);
      auxiliarR.push_back(0.0);
   }
}

void getCSRFormat(double **matrix){
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
}

vector<double> substract(int matrixLength){
   vector<double> auxSub;
   for(int i=0; i < matrixLength; i++){
      auxSub.push_back(0.0);
   }
   for(int i = 0; i < matrixLength; i++){
      //cout << " r: " << r[i] << endl;
      //cout << " auxiliarR: " << auxiliarR[i] << endl;
      auxSub[i] = abs(r[i] - auxiliarR[i]);
   }
   return auxSub;
}

int main(int argc, char** argv) {
   
   initMPI(argc, argv);
   clock_t t1 = clock();
   if(argc != 4){
      cerr << "Error: Invalid arguments. " << endl;
      exit(1);
   }
   matrixLength = atoi(argv[1]);
   matrixDensity = atof(argv[2]);
   tolerance = atof(argv[3]);
   double** matrix;
   matrix = new double*[matrixLength];
   for(int i = 0; i < matrixLength; i++){
      matrix[i] = new double[matrixLength];
   }
   matrix = createMatrix();
   getCSRFormat(matrix);
   initializeR();
   if(world_rank == 0){
      //printMatrix(matrix);
      vector<double>::iterator position;
      vector<double> tmp;
      auxiliarRMPI.resize(matrixLength);
      do{
         for(int i = 0; i < matrixLength; i++){
            auxiliarR[i] = 0.0;
         }
         for(int i = 1; i < world_size; i++){
            MPI_Recv(&auxiliarRMPI[0], matrixLength,  MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //auxiliarR = auxiliarRMPI;
            for(int j = 0; j < matrixLength; j++){
               if(auxiliarRMPI[j] != 0){
                  auxiliarR[j] = auxiliarRMPI[j];
               }
            }
         }
         tmp = auxiliarR;
         auxiliarR = r;
         
         for(int i = 0; i < matrixLength; i++){
            r[i] = 0.0;
         }
         r = tmp;
         for(int i = 1; i < world_size; i++){
            MPI_Send( &r[0], matrixLength, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
         }
         sub = substract(matrixLength);
         position = max_element(sub.begin(), sub.end());
         cout << "Tolerance: "<< tolerance << " Maximum: " << *position << endl;
      }while(tolerance < *position);
      cout << "Vector r: " << endl;
      for (double i : r) {
         cout << i << " ";
      }
      cout << endl;
      r.resize(0);
      for(int i = 1; i < world_size; i++){
         MPI_Recv(&auxiliarRMPI[0], matrixLength,  MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         MPI_Send( &r[0], matrixLength, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
      }
   }else{
      int ini = initial(world_rank);
      int fin = final(world_rank);
      while(true){
         auxiliarRMPI = multiply(ini,fin,world_rank);
         MPI_Send( &auxiliarRMPI[0], matrixLength, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
         MPI_Recv(&r[0], matrixLength,  MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
   }
   // Finalize the MPI environment.
   clock_t t2 = clock();
   double diff = (double)t2 - (double)t1;
   double seconds = diff/CLOCKS_PER_SEC;
   cout << "Time: " << seconds <<"seconds" << endl;
   MPI_Finalize();
}