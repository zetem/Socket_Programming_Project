# Socket-Programming-Project
Implementation of TCP and UDP communication, EE450 USC

client offloads some computation to a server (edge server) which in turn distributes the load               over 2 backend servers, one server performs bitwise "and" and the other performs bitwise "or" operations. The client and the edge server communicate over a TCP connection while the              communication between the edge server and the Back-Servers is connectionless and over UDP.

Client ---TCP--- edge server ----UDP---- bitwise or/and computation 


make all Compiles

make server_or Runs server_or server 

make server_and Runs server_and server 

make edge Runs edge server 

./client <input_file_name> Starts the client with job file like job.txt

