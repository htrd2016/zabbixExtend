CC=gcc
CFLAGS=-g -Wall -O2
LDFLAGS=-L/usr/lib/
LIBS=-L/usr/lib/ -lmysqlclient -lpthread
INCLUDES= -I./mysql-connector-c-6.0.2-linux-rhel5
TARGET=DataToJson

DataToJson: util.o
	$(CC) -o $@ $(CFLAGS) main.c util.o $(LIBS)

util.o:	util.c
	$(CC) $(CFLAGS) $(INCLUDES) -c util.c
	
all:	$(TARGET)

clean:
	rm  -f *.o $(TARGET)
