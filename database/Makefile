dict:loaddata.o
	gcc loaddata.o -o dict -lsqlite3

%.o: %.c
	gcc -c -o $@ $^ -lsqlite3 

clean:
	rm *.o dict
