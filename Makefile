all:
	$(MAKE) -C ./server
	$(MAKE) -C ./client
	$(MAKE) -C ./database

clean:
	$(MAKE) -C ./server clean
	$(MAKE) -C ./client clean
	$(MAKE) -C ./database clean

