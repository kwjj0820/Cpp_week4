FLAGS = -Werror -std=c++11

main.exe: database.o database.h
	g++ -o main.exe database.o database.h

database.o: database.cpp database.h
	g++ ${FLAGS} -c database.cpp