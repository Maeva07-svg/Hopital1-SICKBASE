# Makefile pour SICKBASE avec MSYS2

CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
INCLUDES = -I/mingw64/include/SDL2
LIBS = -L/mingw64/lib

SRCS = src/sdl_ui.c \
       src/consultations.c \
       src/patient.c \
       src/personnel.c \
       src/comptes.c \
       src/ordonnances.c \
       src/diagnostic.c \
       src/interventions.c \
       src/archives.c \
       src/fichiers.c \
       src/fichiers_personnel.c \
       src/infirmier.c \
       src/fichiers_relations.c \
       src/fichiers_rdv.c \
       src/rdv_avance.c \
       src/pharmacie.c \
       src/facturation.c \
       src/laboratoire.c \
       src/recuperation.c \
       src/administration.c \
       src/notification.c \
       src/messagerie.c \
       src/ui.c

OBJS = $(SRCS:.c=.o)
TARGET = sickbase.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(SDL_FLAGS) $(LIBS)
	@echo "Compilation terminée !"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run