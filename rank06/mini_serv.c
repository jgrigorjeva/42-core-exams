#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <strings.h>

void	fatal(void)
{
	write(2, "Fatal error\n", 12);
	exit (1);
}

struct //an unnamed structure
{ 
	int id;
	char *buf;
} clients[65536]; // clients is an array of these structures

void broadcast(int fd, fd_set wr, char *msg, int max_fd)
{
	for (int i = 0; i <= max_fd; i++)
	{
		if (FD_ISSET(i, &wr) && i != fd)
			send(i, msg, strlen(msg), 0);
	}
}

int	extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int		i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char	*str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		return (1);
	}

	int portnum = atoi(argv[1]);
	int sockfd;
	// socklen_t len;
	struct sockaddr_in servaddr; //server
	// struct sockaddr_in cli; //client 

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // server socket
	if (sockfd == -1)
		fatal();
	// //we are not expected to print additional messages
	// else 
	// 	printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
	servaddr.sin_port = htons(portnum);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr,
				sizeof(servaddr))) != 0)
		fatal();
	else
		printf("Socket successfully binded..\n");
	if (listen(sockfd, 10) != 0)
		fatal();
	// {
	// 	printf("cannot listen\n");
	// 	exit(0);
	// }
	// len = (socklen_t)sizeof(cli);
	
	int next_id = 0;
	int max_fd = 0; //if a client disconnects, the next fd can take its fd, but the next id will be the previous client's id+1
	fd_set cur, rd, wr;

	FD_ZERO(&cur);
	FD_SET(sockfd, &cur);

	max_fd = sockfd; // at the start, there is only the server socket, so it is the max
	printf("sockfd: %d\n", sockfd);

	while(1)
	{
		rd = wr = cur; //initiaize the fd sets to the current one
		if (select(max_fd + 1, &rd, &wr, 0, 0)<0)
			continue;
		for (int fd = 0; fd <= max_fd; fd++)
		{
			if (!FD_ISSET(fd, &rd))
				continue;
			if (fd == sockfd)
			{
				printf("client tries to connect\n");
				int c = accept(sockfd, 0, 0); //accept new client
				if (c<0)
					continue; // if accept returns with an error
				if (c > max_fd)
					max_fd = c; // update max_fd with the new fd
				FD_SET(c, &cur); // add new socket to the current set
				clients[c].id = next_id++; // add the new client to the structure
				clients[c].buf = NULL;

				char message[64]; // prepare the welcome message
				bzero(message, 64);
				sprintf(message, "server: client %d just arrived\n", clients[c].id);
				write(1, &message, strlen(message));
				broadcast(fd, wr, message, max_fd);
				// broadcast(fd) the message to all the other clients

				
			}
			else
			{
				char readbuf[65536];
				int n = recv(fd, readbuf, sizeof(readbuf)-1, 0); // read from client
				if (n <= 0) //error or client left
				{
					char message[64];
					bzero(&message, sizeof(message));
					sprintf(message, "server: client %d just left\n", clients[fd].id);
					write(1, &message, strlen(message));
					broadcast(fd, wr, message, max_fd);
					// breadcast message
					free(clients[fd].buf); // free unfinished messages from this client's buffer
					clients[fd].buf = NULL;
					FD_CLR(fd, &cur); // remove client from the furrent fd set
					close(fd);
				}
				else{ //send message
					char *line = NULL;
					char message[65536];
					readbuf[n] = '\0'; // null terminate the message read previously
					printf("message:\n%s", readbuf);
					clients[fd].buf = str_join(clients[fd].buf, readbuf);
					printf("client buffer:\n%s", clients[fd].buf);
					while (extract_message(&(clients[fd].buf), &line))
					{
						sprintf(message, "client %d: %s", clients[fd].id, line);
						write(1, &message, strlen(message));
						broadcast(fd, wr, message, max_fd);
						// broadcast message
						free(line);
						line = NULL;
					}


				}
			}
		}
		
		
	}
	for (int i = 0; i < max_fd; i++) // cleanup
	{
		if (clients[i].buf)
			free(clients[i].buf);
		if (FD_ISSET(i, &cur))
			close(i);
	}
	// connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	// if (connfd < 0)
	// {
	// 	printf("server acccept failed...\n");
	// 	exit(0);
	// }
	// else
	// 	printf("server acccept the client...\n");
}