hello: Main.o Authentication.o sqlite3.o
	gcc Main.o Authentication.o sqlite3.o -o hello
Main.o: Main.c
	gcc Main.c -o Main.o
Authentication.o: Authentication.c
	gcc Authentication.c -o Authentication.o
sqlite3.o: sqlite3.c
	gcc sqlite3.c -o sqlite3.o
clean:
	rm -rf *.o hello	