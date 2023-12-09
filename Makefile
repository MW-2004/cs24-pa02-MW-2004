all: 
	g++  movies.cpp main.cpp -o runMovies -Ofast
tmp1:
	g++  movies.cpp tmp.cpp -o tmp1 -Ofast
	g++  movies.cpp tmp2.cpp -o tmp2 -Ofast