CC = gcc
CFLAGS = -g -Wall
LIBS = -lglut -lGLU -lGL -lm

ARGS = args.txt

all: main cargo_plane collecting_worker_committee distributing_worker_committee family splitting_worker_committee opengl

main: main.o
	$(CC) $(CFLAGS) $< -o $@ 

cargo_plane: cargo_plane.o
	$(CC) $(CFLAGS) $< -o $@
    
opengl: opengl.o
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)    

collecting_worker_committee: collecting_worker_committee.o
	$(CC) $(CFLAGS) $< -o $@

distributing_worker_committee: distributing_worker_committee.o
	$(CC) $(CFLAGS) $< -o $@

family: family.o
	$(CC) $(CFLAGS) $< -o $@

splitting_worker_committee: splitting_worker_committee.o
	$(CC) $(CFLAGS) $< -o $@

run: main
	./main $(ARGS)

find:
	find -name '\tmp\message_queue*' -exec rm {} \;

clean:
	rm -f *.o main cargo_plane collecting_worker_committee distributing_worker_committee family splitting_worker_committee opengl
