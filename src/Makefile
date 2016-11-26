all: pageRank

pageRank: mpi.cpp
	mpiCC -o pageRank mpi.cpp -std=c++11
	
execute: pageRank
	mpirun -np 4 pageRank 10 0.1 0.02
	
clean:
	rm pageRank
