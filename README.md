# myMake
Program simulates the 'make' UNIX command, by calling myMake and then the makefile in the command line

**Example makefile**
```bash
spellcheck.o : utils.h spellcheck.h spellcheck.c
        gcc -Wall -c spellcheck.c

hash.o : hash.c util.h hash.h
        gcc -Wall hash.c

spellcheck : hash.o spellcheck.o
        gcc *.o -o spellcheck"
 ```
 
