client:client.o dict_client.o
	gcc client.o dict_client.o -o client 

%.o: %.c
	gcc -c -o $@ $^ 

clean:
	rm *.o client
