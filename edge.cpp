#include "edge.h"

main()
{
	///////////////// SET UP ///////////////////////
	const char *edge_UDP_port_number = "24585";
	const char *edge_TCP_port_number = "23585";
	const char *and_server_port_number = "22585";
    const char *or_server_port_number = "21585";
    
	int edge_UDP_sock_fd, edge_TCP_sock_fd, child_TCP_sock_fd;
	
	struct addrinfo hints1, hints2, *edge_TCP_servinfo, *edge_UDP_servinfo, *and_servinfo, *or_servinfo;
	////////////////// VARIALBLE //////////////////
	struct sockaddr_storage TCP_addr_storage , addr; // stored client which listen sent
	socklen_t stored_TCP_addr_size, fromlen;
	
	////////////////UDP Socket//////////////
	memset(&hints1, 0, sizeof hints1);
	hints1.ai_family = AF_UNSPEC; // use IPv4 or IPv6 
	hints1.ai_socktype = SOCK_DGRAM;
	hints1.ai_flags = AI_PASSIVE; // use in my IP 
	getaddrinfo( NULL, edge_UDP_port_number, &hints1, &edge_UDP_servinfo); // NULL -> PASSIVE
    getaddrinfo( NULL, and_server_port_number, &hints1, &and_servinfo); // NULL -> PASSIVE
    getaddrinfo( NULL, or_server_port_number , &hints1, &or_servinfo ); // NULL -> PASSIVE

	edge_UDP_sock_fd = socket(edge_UDP_servinfo->ai_family, edge_UDP_servinfo->ai_socktype, edge_UDP_servinfo->ai_protocol);
	if (edge_UDP_sock_fd<0)
	{
		printf("Error creating edge UDP socket to connect to AND server\n");
		exit(1);
	}
	if (bind(edge_UDP_sock_fd, edge_UDP_servinfo->ai_addr, edge_UDP_servinfo->ai_addrlen)<0)
	{
		printf("Error binding edge UDP socket to connect to AND server\n");
		exit(1); 
	}
	
	////////////////TCP Socket/////////////
	memset(&hints2, 0, sizeof hints2);
	hints2.ai_family = AF_UNSPEC; // use IPv4 or IPv6 
	hints2.ai_socktype = SOCK_STREAM;
	hints2.ai_flags = AI_PASSIVE; // use my IP 
	getaddrinfo( NULL, edge_TCP_port_number, &hints2, &edge_TCP_servinfo); // NULL
	edge_TCP_sock_fd = socket(edge_TCP_servinfo->ai_family, edge_TCP_servinfo->ai_socktype, edge_TCP_servinfo->ai_protocol);
	if (edge_TCP_sock_fd<0)
	{
		printf("Error creatig edge TCP socket\n");
		exit(1);
	}
	if (bind(edge_TCP_sock_fd, edge_TCP_servinfo->ai_addr, edge_TCP_servinfo->ai_addrlen)<0)
	{
		printf("Error binding edge TCP socket\n");
		exit(1);
	}

	printf("Edge server is up and running\n");
	//freeaddrinfo(edge_UDP_servinfo);
	freeaddrinfo(edge_TCP_servinfo);
	///////////////////////////////////////////
	if( listen(edge_TCP_sock_fd,4) == -1 )//I manually set backlog 5, but there is only one client, it should be less than 20
	{
		printf("Error listening at EDGE TCP\n");
		exit(2);
	}
	

	// communicate on child socket .....
	

	char client_recv_buffer[2600]; // what is the size max 10 digit recieved 
	//char client_recv_line[2600]; // what is the size max 10 digit recieved 
	
	int bytes_recv_client, line_recv=0;
	char and_buff[2200];
	char or_buff[2200];
	while (1) //recieving multiple clients
	{	
		//if (recvfrom (sock_fd, buffer, 1024, 0, (struct sockaddr*)&sendr, &sizeof(struct sockaddr_in)) < 0){
		//	cout << "error recieving\n" ; 
		//}
		struct sockaddr_storage TCP_addr_storage; // stored client which listen sent
		socklen_t stored_TCP_addr_size;
		stored_TCP_addr_size = sizeof(stored_TCP_addr_size);
		child_TCP_sock_fd = accept(edge_TCP_sock_fd, (struct sockaddr *)&TCP_addr_storage, &stored_TCP_addr_size);
		if (child_TCP_sock_fd == -1)
			continue;

		if (bytes_recv_client = recv(child_TCP_sock_fd, client_recv_buffer, 2600, 0 )>0)
		{
			
			//strcat(client_recv_buffer, client_recv_line);
			printf("server buffer: \n%s", client_recv_buffer);
            //break;
		}
		else if (bytes_recv_client == -1)
			printf("Error recieving from the Client\n");
        else
        {
            //continue;
            //break;   
        } 

        
    
		
		short int and_flag =0 , or_flag =0 ;
		int seqence =0;
		char seq_string[3];
        memset(or_buff ,0,strlen(or_buff ));
        memset(and_buff,0,strlen(and_buff));


		for (int i = 0 ; i < 2600; i++)
		{
            seq_string[1] = seqence%10 + '0';
            seq_string[0] = seqence/10 + '0';
            seq_string[2] = ',';
			
            if ((client_recv_buffer[i] == 'a') && (client_recv_buffer[i+1] == 'n') && (client_recv_buffer[i+2] == 'd'))
			{	
				and_flag = 1;
				strcat(and_buff, seq_string); 
				i = i + 3;
				seqence++;
				printf("It is and seqence %s\n", seq_string );
			}
			else if(( client_recv_buffer[i] == 'o') && (client_recv_buffer[i+1] == 'r'))
			{
				or_flag = 1;
				strcat(or_buff, seq_string);
				i = i + 2;
				seqence++;
				printf("It is or seqence %s\n", seq_string );
			}
			else if ( and_flag )
			{
                if (client_recv_buffer[i] == '\n')
                {
                    and_flag = 0;
                    strcat(and_buff,"\n");
                    printf("%s",  and_buff);
                }
				else if (client_recv_buffer[i] == '0') 
					strcat(and_buff,"0");
				else if (client_recv_buffer[i]=='1')
					strcat(and_buff,"1");
				else if (client_recv_buffer[i]==',')
					strcat(and_buff,",");
			}
			else if ( or_flag )
			{
				if (client_recv_buffer[i] == '\n')
                {
                    or_flag = 0;
                    strcat(or_buff,"\n");
                    printf("%s",  or_buff);
                }
                else if (client_recv_buffer[i] == '0') 
					strcat(or_buff,"0");
				else if (client_recv_buffer[i]=='1')
					strcat(or_buff,"1");
				else if (client_recv_buffer[i]==',')
					strcat(or_buff,",");
			}

				
		}


    int sent_or_server  = sendto(edge_UDP_sock_fd, or_buff , sizeof(or_buff) , 0 , or_servinfo-> ai_addr,  or_servinfo->ai_addrlen);
    if (sent_or_server  == -1)
        printf("Error sending to OR server \n");

    char or_result[1400];
    fromlen = sizeof addr; 
    int recv_or_server = recvfrom(edge_UDP_sock_fd ,or_result, sizeof(or_result) ,0, or_servinfo->ai_addr, &fromlen);
    if (recv_or_server == -1)
        printf("Error recieving result form AND server at edge\n");
    
    printf("OR result received is \n%s", or_result );
 
    int sent_and_server = sendto(edge_UDP_sock_fd, and_buff, sizeof(and_buff), 0 , and_servinfo-> ai_addr, and_servinfo->ai_addrlen);
    if (sent_and_server == -1)
        printf("Error sending to And server\n");

    char and_result[1400];
    fromlen = sizeof addr; 
    int recv_and_server = recvfrom(edge_UDP_sock_fd ,and_result, sizeof(and_result) ,0, and_servinfo->ai_addr, &fromlen);
    if (recv_and_server == -1)
        printf("Error recieving result form AND server at edge\n");
    
    printf("AND result received is \n%s\n", and_result );

    char result[2800];
    memset(result,0,strlen(result));
    strcat(result,or_result);
    strcat(result,and_result);

    int bytes_sent_TCP = send(child_TCP_sock_fd, result, strlen(result), 0);
    if (bytes_sent_TCP == -1)
        printf("Error sending result to the client\n");
    printf("bytes sent = %d\n",bytes_sent_TCP );
    printf("Sent to Client:\n%s",result );
    close(child_TCP_sock_fd);
    
    }

return 0;
} 