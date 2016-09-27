/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		file_clnt.c -   A simple file transfer client using TCP
--
--	PROGRAM:		fclnt.exe (?)
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
--	The program will make TCP connections to a server specified by user.
-- 	The server can be specified using a fully qualified domain name or and
--	IP address. After the connection has been established the user will be
-- 	prompted for a command; get or receive file. The command is then sent to the server and the
-- 	response(file) from the server is displayed.
---------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_TCP_PORT		7005	// Default port
#define BUFLEN			80  	// Buffer length
#define GET			0 
#define SEND			1 

int main (int argc, char **argv)
{
	//initialize variables
	int n, bytes_to_read;
	int sd, port;
	struct hostent	*hp;
	struct sockaddr_in server;
	char  *host, *bp, rbuf[BUFLEN], sbuf[BUFLEN], **pptr, *sptr;
	char str[16];
	int result;
	
	//get port from cmd args

	//temp
	switch(argc)
	{
		case 2:
			host =	argv[1];	// Host name
			port =	SERVER_TCP_PORT;
		break;
		case 3:
			host =	argv[1];
			port =	atoi(argv[2]);	// User specified port
		break;
		default:
			fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
			exit(1);
	}

	//create socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host)) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	//connect to server
	if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
	pptr = hp->h_addr_list;
	printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));
	printf("Enter User Command (get or send):\n");

	// get user's text
	fgets (sbuf, BUFLEN, stdin);

	//transfer string command for use in switch case
	if(strcmp(sbuf, "get"))	{result = 0;}
	if(strcmp(sbuf, "send")){result = 1;}

	//switch statement for get or receive command
	switch(result){
		case GET:
			//testing, TODO: get filename from args, send file name to server
			send (sd, sbuf, BUFLEN, 0);
			printf("Getting file from server...\n");
			break;

		case SEND:
			//testing, TODO: create file, send to server
			send (sd, sbuf, BUFLEN, 0);
			printf("Sending file to server...\n");
			break;

		default:
			fprintf(stderr, "Incorrect command.\n", argv[0]);
			exit(1);
	}

	//!!!temp for testing
	// Transmit data through the socket
	//send (sd, sbuf, BUFLEN, 0);

	printf("Received command:\n");
	bp = rbuf;
	bytes_to_read = BUFLEN;

	// client makes repeated calls to recv until no more data is expected to arrive.
	n = 0;
	while ((n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN)
	{
		bp += n;
		bytes_to_read -= n;
	}
	printf ("%s\n", rbuf);
	fflush(stdout);
	close (sd);
	return (0);
}






