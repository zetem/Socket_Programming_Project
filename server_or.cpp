#include "server_or.h"

char or_func(char a, char b)
{
	if ( (a == '0') && (b=='0'))
		return '0';
	else 
		return '1';
}

int main()
{
	
	const char *or_port_number = "21585"; // between 1024 to 65535
	const char *edge_UDP_port_number = "24585";

	int or_server_sock_fd;
	
	struct addrinfo hints, *or_servinfo, *edge_servinfo;
	struct sockaddr_storage addr;
	socklen_t fromlen;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP address	
	getaddrinfo( NULL, or_port_number, &hints, &or_servinfo);
	getaddrinfo( NULL, edge_UDP_port_number, &hints, &edge_servinfo);

	///// creating or server socket
	or_server_sock_fd = socket( or_servinfo->ai_family , or_servinfo->ai_socktype, or_servinfo->ai_protocol);
	if (or_server_sock_fd < 0){
		printf("Error opening socket at 'or' server side\n" );
		exit(3);
	}
	
	///// binfing and server socket 
	if (bind(or_server_sock_fd, or_servinfo->ai_addr, or_servinfo->ai_addrlen)<0){
		printf("Error binding at 'or' server\n" );
		exit(3);
	}

	printf("The Server OR is up and runnig using UDP on port %s \n", or_port_number );
	freeaddrinfo(or_servinfo); 

	char or_buffer[2500];
	char result[1400]; // 10 digit + 2 seqence number + , + newline
	int e;
	char op[2][10];
	char tmp[10]; 
	int len_op[2];
	int k , result_p , length ;
	while(1) // doing nothing 
	{	
		fromlen = sizeof addr;
		e = recvfrom(or_server_sock_fd ,or_buffer,sizeof(or_buffer),0, edge_servinfo->ai_addr, &fromlen );
		printf("%s", or_buffer );
		
		len_op[0]=0; len_op[1]=0; k =0; result_p=3;
		result[0] = or_buffer[0];
		result[1] = or_buffer[1];
		result[2] = or_buffer[2];
		for (int i=3; i< strlen(or_buffer); i++)
		{
			op[1][len_op[1]] = '\0';
			if (or_buffer[i] == '\n')
			{
				if ( len_op[0] > len_op[1])
				{
					length = len_op[0];
					strcpy(tmp, op[1]);
					for ( int j =0 ; j< (length - len_op[1]); j++) ///notice ;
					op[1][j] = '0';
					strcat(op[1],tmp);
				}
				else 
				{
					length = len_op[1];
					strcpy(tmp, op[0]);
					for ( int j =0 ; j< (length - len_op[0]); j++) ///notice ;
					op[0][j] = '0';
					strcat(op[0],tmp);					
				}
				//printf("%s and %s\n", op[0], op[1] );
				//printf("lenght is : %d\n", length );

				for ( int y=0 ; y<length ; y++)
				{
					result[result_p] = or_func(op[0][y], op[1][y]);
					//printf("%c\n",result[result_p]);
					result_p++;
				}
				result[result_p] = '\n'; 

				k =0; 
				len_op[0] =0;
				len_op[1] =0; 
				memset(op[0],0,strlen(op[0]));
				memset(op[1],0,strlen(op[1]));
				result[result_p+1] = or_buffer[i+1];
				result[result_p+2] = or_buffer[i+2];
				result[result_p+3] = or_buffer[i+3];
				result_p = result_p + 4; 
				i = i+3;
			}
			else if (or_buffer[i]==',')
			{
				op[0][len_op[0]] = '\0';  
				k =1;
			}
			else if ((or_buffer[i] == '0') || (or_buffer[i] == '1'))
			{
				op[k][len_op[k]] = or_buffer[i];
				len_op[k]++; 
			}
		}

	printf("Result is: \n%s", result );
	int sent_bytes = sendto(or_server_sock_fd, result, sizeof(result), 0 , edge_servinfo-> ai_addr, edge_servinfo->ai_addrlen);
	if (sent_bytes == -1)
		printf("Error sending result form OR server\n");
	}
}