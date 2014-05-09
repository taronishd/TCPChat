#Taronish Daruwalla

p2pim: main.o udp.o
	g++ -Wall -g -ansi -o p2pim main.o udp.o

main.o: main.cpp udp.h
	g++ -Wall -g -ansi -c main.cpp

udp.o: udp.cpp udp.h
	g++ -Wall -g -ansi -c udp.cpp

clean:
	rm -f p2pim *.o
