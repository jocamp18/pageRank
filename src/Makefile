all: pageRank serialPageRank

pageRank: mpi.cpp
	mpiCC -o pageRank mpi.cpp -std=c++11
	
serialPageRank: Matrix.cpp
	g++ -o serialPageRank Matrix.cpp --std=c++11
	
execute: pageRank
	mpirun -np 5 --hostfile ../config/hosts pageRank 6000 0.1 0.003
	
clean:
	rm pageRank
	rm serialPageRank
