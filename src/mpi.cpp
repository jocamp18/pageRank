#include <mpi.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    if(world_rank == 0){
   int matrixLength = atoi(argv[1]);
    double matrixDensity = atof(argv[2]);
    //int matrix[5][5] = {{1,-1,0,-3,0},{-2,5,0,0,0},{0,0,4,6,4},{-4,0,2,7,0},{0,8,0,0,-5}};
    double matrix[matrixLength][matrixLength];
    double auxArray[matrixLength];
    double val;
   vector<float> values;
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
   int cont1 = 0;
   for(int i = 0; i < pointerB.size();i++){
      int min = pointerB[i];
      int max = pointerE[i];
      for(int j = min; j < max;j++){
         cout << "cont " << cont1 << " send " << values[j] << endl;
      }
      cont1++;
   }
    cout << "values: ";
   for (double n : values){
      cout << n << ' ';
   }
    }
    
    
    // Print off a hello world message
    cout << "Hello from " << processor_name << " rank " << world_rank << " of " << world_size << endl;

    // Finalize the MPI environment.
    MPI_Finalize();
}