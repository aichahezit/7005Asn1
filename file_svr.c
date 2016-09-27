/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		file_svr.c -   A simple file transfer server using TCP
--
--	PROGRAM:		fsvr.exe (?)
--
--	FUNCTIONS:		Berkeley Socket API
--
--	DATE:			September 27, 2016
--
--	REVISIONS:		N/A
--
--	DESIGNERS:		Aicha Hezit
--
--	PROGRAMMERS:		Aicha Hezit
--
--	NOTES:
--	The program will accept TCP connections from client machines.
-- 	The program will recieve commands from the client socket and either send or 
-- 	recieve a file back.
---------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_TCP_PORT 7005	// Default port
#define BUFLEN	80		//Buffer length


int main (int argc, char **argv)
{

	//initialize variables
	int	n, bytes_to_read;
	int	sd, new_sd, client_len, port;
	struct	sockaddr_in server, client;
	char	*bp, buf[BUFLEN];

	//get cmd args for port
	port = atoi(argv[1]);

	//create stream socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("Can't create a socket");
		exit(1);
	}

	//bind address to socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}	

	//listen for connections
	listen(sd, 3);

	while(1){
	
		//initialize client
		client_len= sizeof(client);
		
		//accept client
		if ((new_sd = accept (sd, (struct sockaddr *)&client, &client_len)) == -1)
			{
				fprintf(stderr, "Can't accept client\n");
				exit(1);
			}
		
		//temp for testing, replace with confirmation of recieved user command
		bp = buf;
		bytes_to_read = BUFLEN;
		while ((n = recv (new_sd, bp, bytes_to_read, 0)) < BUFLEN)
		{
			bp += n;
			bytes_to_read -= n;
		}
		printf ("sending:%s\n", buf);

		send (new_sd, buf, BUFLEN, 0);
		close (new_sd);
	}

	close(sd);
	return(0);	
}
