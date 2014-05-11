#Taronish Daruwalla

p2pim: main.o udp.o tcp.o
	g++ -Wall -g -ansi -o p2pim main.o udp.o tcp.o

main.o: main.cpp udp.h tcp.h
	g++ -Wall -g -ansi -c main.cpp

udp.o: udp.cpp udp.h
	g++ -Wall -g -ansi -c udp.cpp

tcp.o: tcp.cpp tcp.h
	g++ -Wall -g -ansi -c tcp.cpp

clean:
	rm -f p2pim *.o
