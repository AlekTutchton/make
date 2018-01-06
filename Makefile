mymake : mymake.o buildStruct.o
	gcc -g -Wall mymake.o buildStruct.o -L/home/cs352/spring17/Assignments/proj09 -lcs352 -o mymake

