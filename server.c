/* server.c
 * CJ Miller, Jesse Shellabarger
 * Did the C and Echo Incentives
 */

#include "globals.h"

static void usage();
static void die(const char* message);
static unsigned long resolveName(const char* name);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serverAddr;
	char *serverName;
	unsigned short serverPort;
	isVerbose = 0;

	serverName = "localhost";
	serverPort = DEFAULTPORT;

	int ch;
	while ((ch = getopt(argc, argv, "h:p:uv")) != -1) {
		switch (ch) {
		case 'h':
			serverName = optarg;
			break;
		case 'p':
			serverPort = atoi(optarg);
			break;
		case 'u':
			usage();
			break;
		case 'v':
			isVerbose = 1;
			break;
		default:
			usage();
			return 0;
		}
	}
	if (isVerbose)
	{
		printf("Using port %d\n", serverPort);
	}

	printf("We did the echo incentive.\n");

	//Create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0) {
        perror("Unable to create socket.");
        return 1;
    }

	//Initialize the endpoint structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_addr.s_addr = resolveName(serverName);
	serverAddr.sin_port        = htons(serverPort);

    int bound = bind(sock, (void*)&serverAddr, sizeof(serverAddr));
    if(bound < 0) {
        perror("Unable to bind to port");
        return 1;
    }

    int listening = listen(sock, 10);
    if(listening < 0) {
        perror("Unable to start listening for clients");   
        return 1;
    }

    char buffer[BUFFER_SIZE + 1];
    char* serverText;
    serverText = "Server: ";
	while (1)
	{
		socklen_t len = sizeof(serverAddr);
        int client = accept(sock, (void*)&serverAddr, &len);

        if(client < 0)
            continue;

        int receive;
        while ((receive = recv(client, buffer, BUFFER_SIZE, 0)) > 0) {
        	buffer[receive] = '\0';
        	printf("%s",buffer);

        	char toReturn[receive + strlen(serverText)];
        	strcpy(toReturn, serverText);
        	strcat(toReturn, buffer);

        	send(client, toReturn, strlen(toReturn), 0);
        	printf("Sending: %s\n", toReturn);
        }
        close(client);
	}

	close(sock);
	return 0;
}

static void usage()
{
	fprintf(stderr, "Usage: server [-u] [-v] [-h <hostname>] [-p <port>]\n");
	exit(1);
}

static void die(const char* message)
{
	perror(message);
	exit(1);
}

static unsigned long resolveName(const char* name)
{
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
		die("gethostbyname() failed");

	return *((unsigned long*)host->h_addr_list[0]);
}
