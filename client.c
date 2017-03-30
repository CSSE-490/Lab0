/* client.c
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
	unsigned short serverPort;
	char *serverName;

	serverName = "localhost";
	serverPort = DEFAULTPORT;
	isVerbose = 0;

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
		printf("Using server %s\n", serverName);
		printf("Using port %d\n", serverPort);
	}

	//Create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	//Initialize the endpoint structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_addr.s_addr = resolveName(serverName);
	serverAddr.sin_port        = htons(serverPort);

	int connected = connect(sock, (void*)&serverAddr, sizeof(serverAddr));
	if(connected < 0) {
		perror("Unable to connect to server");
		return 1;
	}

	char buffer[BUFFER_SIZE + 1];
	while (1)
	{
		printf("Please enter text to send to server.\n");
		char input[BUFFER_SIZE];
		//Prompt for the input string
        fgets(input, BUFFER_SIZE, stdin);
		//If no more input, break
		if(strcmp(input, ";;;\n") == 0) {
			break;
		}
		//Send the string to the server
		int sent = send(sock, input, strlen(input), 0);
		if(sent < 0) {
			perror("Error sending");
			return 1;
		}

		int received = recv(sock, buffer, BUFFER_SIZE, 0);
		if(received < 0) {
			perror("Server Disconnected");
			return 1;
		}
		buffer[received] = '\0';

		printf("%s\n", buffer);
	}

	close(sock);
	return 0;
}

static void usage()
{
	fprintf(stderr, "Usage: client [-u] [-v] [-h <server>] [-p <port>]\n");
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
