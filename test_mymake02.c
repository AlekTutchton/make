spellcheck.o : utils.h spellcheck.h spellcheck.c
	gcc -Wall -c spellcheck.c

hash.o : hash.c util.h hash.h
	gcc -Wall hash.c

spellcheck : hash.o spellcheck.o
	gcc *.o -o spellcheck

