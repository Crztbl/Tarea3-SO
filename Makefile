tarea3: tarea3.o sthread.o
	g++ tarea3.o sthread.o -o tarea3 -lpthread

tarea3.o: tarea3.cpp
	g++ -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS tarea3.cpp -c -o tarea3.o

sthread.o: sthread.c sthread.h
	g++ -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS sthread.c -c -o sthread.o

clean:
	rm -f programa *.o