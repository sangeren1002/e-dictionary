e-dictionary
This repository is a simple online e-dictionary.

First, compile and run the program server.c to create database and start the TCP/IP server. 
Second,import the words and explaination into the sqlite database with compiling and running dict.c about 3-4 minutes.It all depends. 
Third,compile and run the program client.c 
After all above,you can follow the hints to somethin like register login and searching word,etc. 
Also,you can have a look at the search history.

Actually,I have made it easy. When you get this code you just need to do as follows: 
1.run the command "make clean" 
2.run the command "bash server.sh" 
3.open another terminal and enter the path where the code is. 
4.run the command "bash client.sh"

Support:Linux System