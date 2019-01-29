#all:

client:	client.cpp	
			g++ client.cpp -o client

edge:		edge.cpp
			g++ edge.cpp -o edge

or:		server_or.cpp
			g++ server_or.cpp	-o server_or

and:		server_and.cpp	
			g++ server_and.cpp	-o server_and
