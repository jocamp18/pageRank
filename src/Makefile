all: pageRank

pageRank: mpi.cpp
	mpiCC -o pageRank mpi.cpp -std=c++11
	
execute: pageRank
	mpirun -np 4 pageRank 1000 0.1 0.0002
	
clean:
	rm pageRank
