all: id3.c rps.c
	gcc -o rps rps.c
	gcc -o id3 id3.c

id3: id3.c
	gcc -o id3 id3.c

rps: rps.c
	gcc -o rps rps.c
