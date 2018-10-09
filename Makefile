ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: game.exe

game.exe: game.o
	gcc -o game.exe game.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

game.o: game.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c game.c
	
clean:
	del game.o 
	del game.exe
