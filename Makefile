CC=gcc -g3 -w -o $@
COMPILE=-c $<
VPATH=include
SERVER=server
CLIENT=client


all: ${SERVER} ${CLIENT}


#Build server
${SERVER}: server.o TCPSocket.o
	${CC} $^
server.o: server.c
	${CC} -I ${VPATH} ${COMPILE}
TCPSocket.o: implement/TCPSocket.c
	${CC} -I ${VPATH} ${COMPILE}

#Build client
${CLIENT}: client.o TCPSocket.o
	${CC} $^
client.o: client.c
	${CC} -I ${VPATH} ${COMPILE}

clean:
	rm -rf *.so *.o ${SERVER} ${CLIENT}
