#include "server_and.h"

//using namespace std;

char and_func(char a, char b)
{
	if ( (a == '1') && (b=='1'))
		return '1';
	else 
		return '0';
}

int main()
{
	
	const char *and_port_number = "22585"; // between 1024 to 65535
	const char *edge_UDP_port_number = "24585";
	int and_server_sock_fd;
	
	struct addrinfo hints, *and_servinfo, *edge_servinfo;
	struct sockaddr_storage addr;
	socklen_t fromlen;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP address	
	getaddrinfo( NULL, and_port_number, &hints, &and_servinfo);
	getaddrinfo( NULL, edge_UDP_port_number, &hints, &edge_servinfo);

	///// creating and server socket
	and_server_sock_fd = socket( and_servinfo->ai_family , and_servinfo->ai_socktype, and_servinfo->ai_protocol);
	if (and_server_sock_fd < 0){
		printf("Error opening socket at 'and' server side\n" );
		exit(2);
	}
	
	///// binfing and server socket 
	if (bind(and_server_sock_fd, and_servinfo->ai_addr, and_servinfo->ai_addrlen)<0){
		printf("Error binding at 'and' server\n" );
		exit(2);
	}

	printf("The Server And is up and runnig using UDP on port %s \n", and_port_number );
	freeaddrinfo(and_servinfo); 

    /////////////////// receiving and sendig ////////////////////
	int e;
	char and_data[2500];
	char result[1400]; // 10 digit + 2 seqence number + , + newline
	char op[2][10];
	char tmp[10]; 
	int len_op[2];
	int k , result_p , length ;
	
	while(1) // doing nothing 
	{	
		fromlen = sizeof addr;
		e = recvfrom(and_server_sock_fd ,and_data, sizeof(and_data) ,0, edge_servinfo->ai_addr, &fromlen); //edge_servinfo->ai_addr //(struct sockaddr *)&addr
		if (e == -1)
			printf("Error receiving at AND server\n");
		else if (e == 0)
			continue;

		printf("%s", and_data );
		len_op[0]=0; len_op[1]=0; k =0; result_p=3;
		result[0] = and_data[0];
		result[1] = and_data[1];
		result[2] = and_data[2];
		for (int i=3; i< strlen(and_data); i++)
		{
			op[1][len_op[1]] = '\0';
			if (and_data[i] == '\n')
			{
				if ( len_op[0] > len_op[1])
				{
					length = len_op[1];
					strcpy(tmp, op[0]);
					memset(op[0],0,strlen(op[0]));
					for ( int j =0 ; j< length ; j++) ///notice ;
					op[0][j] = tmp[j+(len_op[0] - length)];
				}
				else 
				{
					length = len_op[0];
					strcpy(tmp, op[1]);
					memset(op[1],0,strlen(op[1]));
					for ( int j =0 ; j< length ; j++) ///notice ;
					op[1][j] = tmp[j+(len_op[1]-length)];		
				}

				//printf("%s and %s\n", op[0], op[1] );
				//printf("length is : %d\n", length );

				for ( int y=0 ; y<length ; y++)
				{
					result[result_p] = and_func(op[0][y], op[1][y]);
					//printf("%c\n",result[result_p]);
					result_p++;
				}
				result[result_p] = '\n'; 

				k =0; 
				len_op[0] =0;
				len_op[1] =0; 
				memset(op[0],0,strlen(op[0]));
				memset(op[1],0,strlen(op[1]));
				result[result_p+1] = and_data[i+1];
				result[result_p+2] = and_data[i+2];
				result[result_p+3] = and_data[i+3];
				result_p = result_p + 4; 
				i = i+3;
				//printf(" Result is: \n%s", result );
			}
			else if (and_data[i]==',')
			{
				op[0][len_op[0]] = '\0';  
				k =1;
			}
			else if ((and_data[i] == '0') || (and_data[i] == '1'))
			{
				op[k][len_op[k]] = and_data[i];
				len_op[k]++; 
			}
		}

	printf(" Result is: \n%s", result );
	int sent_bytes = sendto(and_server_sock_fd, result, sizeof(result), 0 , edge_servinfo-> ai_addr, edge_servinfo->ai_addrlen);
	if (sent_bytes == -1)
		printf("Error sending result form AND server\n");
	}

}
