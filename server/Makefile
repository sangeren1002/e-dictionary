server:server.o dict_server.o
	gcc server.o dict_server.o -o server -lsqlite3

%.o: %.c
	gcc -c -o $@ $^ -lsqlite3

clean:
	rm *.o server *.db
