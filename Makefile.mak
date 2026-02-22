# Makefile pour compiler le projet complet
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
LDFLAGS = 
TARGET = hopital.exe
OBJS = main.o utils.o \
       patient.o fichiers.o \
       personnel.o fichiers_personnel.o \
       consultations.o ordonnances.o interventions.o archives.o \
       fichiers_relations.o diagnostic.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c utils.h patient.h diagnostic.h fichiers.h personnel.h \
        fichiers_personnel.h consultations.h ordonnances.h interventions.h \
        archives.h fichiers_relations.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

patient.o: patient.c patient.h utils.h fichiers.h diagnostic.h
	$(CC) $(CFLAGS) -c patient.c

fichiers.o: fichiers.c fichiers.h patient.h utils.h
	$(CC) $(CFLAGS) -c fichiers.c

personnel.o: personnel.c personnel.h utils.h fichiers_personnel.h
	$(CC) $(CFLAGS) -c personnel.c

fichiers_personnel.o: fichiers_personnel.c fichiers_personnel.h personnel.h utils.h
	$(CC) $(CFLAGS) -c fichiers_personnel.c

consultations.o: consultations.c consultations.h ordonnances.h interventions.h \
                 fichiers_relations.h utils.h patient.h personnel.h
	$(CC) $(CFLAGS) -c consultations.c

ordonnances.o: ordonnances.c ordonnances.h fichiers_relations.h utils.h patient.h personnel.h
	$(CC) $(CFLAGS) -c ordonnances.c

interventions.o: interventions.c interventions.h fichiers_relations.h utils.h patient.h personnel.h
	$(CC) $(CFLAGS) -c interventions.c

archives.o: archives.c archives.h fichiers_relations.h utils.h patient.h consultations.h
	$(CC) $(CFLAGS) -c archives.c

fichiers_relations.o: fichiers_relations.c fichiers_relations.h consultations.h \
                      ordonnances.h interventions.h archives.h
	$(CC) $(CFLAGS) -c fichiers_relations.c

diagnostic.o: diagnostic.c diagnostic.h patient.h fichiers.h utils.h
	$(CC) $(CFLAGS) -c diagnostic.c

clean:
	del *.o *.exe 2>nul || rm -f *.o *.exe

run: $(TARGET)
	$(TARGET)