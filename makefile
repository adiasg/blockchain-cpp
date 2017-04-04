objects = hash.o id.o block.o crypto.o bdb.o

server.out : $(objects) server.o
	g++ -o server.out $(objects) server.o -lcrypto -ldb

client.out : $(objects) client.o
	g++ -o client.out $(objects) client.o -lcrypto -ldb

hash.o : hash.cpp hash.h
	g++ -c hash.cpp

id.o : id.cpp id.h
	g++ -c id.cpp

block.o : block.cpp block.h hash.h id.h id.cpp hash.cpp crypto.h crypto.cpp
	g++ -c block.cpp

crypto.o : crypto.cpp crypto.h
	g++ -c crypto.cpp

bdb.o : bdb.cpp bdb.h block.h block.cpp id.h id.cpp
	g++ -c bdb.cpp

server.o : $(objects) server.cpp
	g++ -c server.cpp

client.o : $(objects) client.cpp
	g++ -c client.cpp

clean :
	rm $(objects) server.o* client.o*
