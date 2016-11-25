all: pageRank

pageRank: Matrix.o
	g++ -o pageRank Matrix.o
	
Matrix.o: Matrix.cpp
	g++ -c -Wall --std=c++11 Matrix.cpp -o Matrix.o
clean:
	rm pageRank
	rm *.o
