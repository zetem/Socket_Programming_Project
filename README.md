# Socket-Programming-Project
implementation of TCP and UDP communication, EE450 USC

client offloads some computation to a server (edge server) which in turn distributes the load               over 2 backend servers, one server performs bitwise "and" and the other performs bitwise "or" operations. The client and the edge server communicate over a TCP connection while the              communication between the edge server and the Back-Servers is connectionless and over UDP.
Client ---TCP--- edge server ----UDP---- bitwise or/and computation 
