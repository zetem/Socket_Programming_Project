#include "client.h"


int main()
{
    FILE *job; 
    char job_buff_line[25]; // and =3 , commas = 2, 2 x operand = 10 => max = 3 + 2 + 2 x 10 = 25 
    char job_buff[2600];
    char tmp_buff[5];
    char res_buffer[100][3]; // only ten digits, means 2bytes
    char res[100][2];
    int client_sock_fd, bytes_sent, bytes_recieved, total_bytes_sent, total_bytes_recieved, line_count, recv_line_count;
    // const char *bit_rep[16] = {
    // [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    // [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    // [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    // [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
    
    // };
    
    struct addrinfo hints, *client_serverinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo( NULL, "23585", &hints, &client_serverinfo);   

    client_sock_fd = socket(client_serverinfo->ai_family, client_serverinfo->ai_socktype, client_serverinfo->ai_protocol);
    if(client_sock_fd<0)
    {
        printf("Error opening socket at client side\n");
        exit(0);
    }
    
    if( connect(client_sock_fd, client_serverinfo->ai_addr, client_serverinfo->ai_addrlen) == -1)
    {
        printf("Error connecting the Client\n");
        exit(0);
    }
    
    
    printf("The client is up and running\n");


    //IF READIGN A FILE
    job = fopen("job.txt","r");
    if (job==NULL)
        exit(0);    

    line_count =0;
    while( fgets(job_buff_line, sizeof(job_buff_line), (FILE*)job) != NULL)
    {
        if (job_buff_line[0] == '\n')
            continue;
        
        strcat(job_buff, job_buff_line);
        //strcat(job_buff, "@");
        line_count++;
        //printf("buffer content is at line count %d : \n%s",line_count,job_buff);
        //printf("char = %c\n",job_buff[0] ); 
    }
   
    int len = strlen(job_buff);
    total_bytes_sent = 0;
        do
        { 
            bytes_sent = send(client_sock_fd, job_buff, len, 0);
            if (bytes_sent== -1)
            {
                printf("error sending from client to edge server\n");
                exit(0);
            }

            total_bytes_sent += bytes_sent;
            printf ("sent %d bytes of whole %d bytes totally = %d,  at line %d \n", bytes_sent, len, total_bytes_sent, line_count);
        }
        while (total_bytes_sent < len);

        //if (recv(client_sock_fd, tmp_buff, 5,0) == -1)
        //    printf("Error receiving at Client\n");
        //else 
        //    printf("%s is recieved\n", tmp_buff );
        //if (tmp_buff == " recv")

        // while(1)
        // {
        //     recv(client_sock_fd, tmp_buff, 5,0);
            
        //     if (strcmp(tmp_buff,"recv") != 0)
        //     {
        //         strcpy(tmp_buff,"nrcv");
        //         printf("signal received and temp is: %s\n", tmp_buff);

        //         break;
        //     }
        // }
    

    printf("The client has successfully finished sending %d lines to the edge servee\n", line_count);
    
    // total_bytes_recieved =0;
    // recv_line_count =0;
    // while( bytes_recieved = recv(client_sock_fd, res_buffer[recv_line_count], 3, 0) )
    // {
    //     if (bytes_recieved == -1)
    //     {
    //         printf("error recieving data from edge server at the client\n");
    //         exit(0);
    //     }
    //     total_bytes_recieved += bytes_recieved;
    //     if (total_bytes_recieved == 3)
    //     {   
    //         printf("the result is sequence#%s: %s%s%s \n",res_buffer[recv_line_count][2] , bit_rep[res_buffer[recv_line_count][1] & 0x03] ,bit_rep[res_buffer[recv_line_count][0] >> 4], bit_rep[recv_line_count][res_buffer[0] & 0x0F] );
    //         total_bytes_recieved = 0;
    //         recv_line_count++;
    //     }

    // }
    // printf("The client has successfully finished recieving all computaion resluts from edge server\n");

    // for (int i =0 ; i<recv_line_count ; i++)
    // {
    //     for (int j=0 ; j<recv_line_count ; j++)
    //         if (bytes_recieved[j][0] == i+48)
    //         {
    //             result[i][0] = bytes_recieved[j][0];
    //             result[i][1] = bytes_recieved[j][1];
    //         }
    // }

    // printf("The final computaion resluts are:\n");
    // for (int i =0 ; i<recv_line_count ; i++)
    // {
    //     printf(" %s%s%s \n" , bit_rep[resluts[i][1] & 0x03] ,bit_rep[resluts[i][0] >> 4], bit_rep[resluts[i][0] & 0x0F] );

    // }
    char result[2800];
    char reslut_tmp[2800];
    memset(result,0, strlen(result));
    int recv_bytes;
    while (recv_bytes = recv(client_sock_fd, reslut_tmp, strlen(reslut_tmp), 0 ))
    {
        if (recv_bytes == -1)
        {
            printf("Error receiving at client\n");
            exit(3);
        }
        strcat(result, reslut_tmp);
    }

    printf("bytes received %d\n", recv_bytes );
    printf("At client:\n%s\n",result );

    char s[2];
    char sorted_result[100][10];
    short int flag_new_line=1;
    int place, line_number, result_line_count=0;
    for (int l = 0; l<strlen(result); l++)
    {
        if (flag_new_line)
        {
            line_number = (result[l+1]-'0') + (result[l]-'0')*10;
            printf( "line_number is %d and char = %c\n", line_number, result[l]);
            flag_new_line =0;
            place = 0;
            l = l+2;
        }
        else if( result[l] == '\n')
        {
            flag_new_line =1;
            result_line_count++; 
        }
        else 
        {
        if ((place==0) && (result[l]=='0'))
            continue;
        sorted_result[line_number][place]= result[l];
        printf( "stored char = %c and string %s\n", result[l], sorted_result[line_number] );
        place++;
        }
    }

    for (int y=0 ; y <result_line_count ; y ++)
    {
        printf("result#%d = %s\n", y, sorted_result[y]);
    }

    fclose(job);
    close(client_sock_fd);
    freeaddrinfo(client_serverinfo);

    return 0;
}