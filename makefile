objects = hash.o id.o block.o crypto.o bdb.o

server.out : $(objects) server.o
	g++ -o server.out $(objects) server.o -lcrypto -ldb

client.out : $(objects) client.o
	g++ -o client.out $(objects) client.o -lcrypto -ldb

test-block.out : $(objects) test-block.o
	g++ -o test-block.out $(objects) test-block.o -lcrypto -ldb

test-block-db.out : $(objects) test-block-db.o
	g++ -o test-block-db.out $(objects) test-block-db.o -lcrypto -ldb

test-block-db-ip.out : $(objects) test-block-db-ip.o
	g++ -o test-block-db-ip.out $(objects) test-block-db-ip.o -lcrypto -ldb

hash.o : hash.cpp hash.h block.h block.cpp
	g++ -c hash.cpp

id.o : id.cpp id.h
	g++ -c id.cpp

block.o : block.cpp block.h hash.h id.h id.cpp hash.cpp crypto.h crypto.cpp
	g++ -c block.cpp

crypto.o : crypto.cpp crypto.h
	g++ -c crypto.cpp

bdb.o : bdb.cpp bdb.h block.h block.cpp
	g++ -c bdb.cpp

server.o : $(objects) server.cpp
	g++ -c server.cpp

client.o : $(objects) client.cpp
	g++ -c client.cpp

test-block.o : $(objects) test-block.cpp
	g++ -c test-block.cpp

test-block-db.o : $(objects) test-block-db.cpp
	g++ -c test-block-db.cpp

test-block-db-ip.o : $(objects) test-block-db-ip.cpp
	g++ -c test-block-db-ip.cpp

clean :
	rm $(objects) server.o* client.o* test-block.o* test-block-db.o*
