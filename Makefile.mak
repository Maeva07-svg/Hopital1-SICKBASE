# Makefile pour compiler le projet
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 
TARGET = hopital.exe
OBJS = main.o patient.o diagnostic.o utils.o fichiers.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c utils.h patient.h diagnostic.h fichiers.h
	$(CC) $(CFLAGS) -c main.c

patient.o: patient.c patient.h utils.h fichiers.h diagnostic.h
	$(CC) $(CFLAGS) -c patient.c

diagnostic.o: diagnostic.c diagnostic.h patient.h fichiers.h utils.h
	$(CC) $(CFLAGS) -c diagnostic.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

fichiers.o: fichiers.c fichiers.h patient.h utils.h
	$(CC) $(CFLAGS) -c fichiers.c

clean:
	del *.o *.exe 2>nul || rm -f *.o *.exe

run: $(TARGET)
	$(TARGET)