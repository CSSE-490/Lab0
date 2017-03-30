/* globals.h
 * Kevin Wells, Spring 2012
 */

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#define DEFAULTPORT 5555
#define LOADSIZE 30
#define BUFFER_SIZE 60000

extern int isVerbose;

typedef enum
{
	ACK,     /* acknowledge a packet in a file transfer */
	DATA,    /* A chunk of a file */
	ECHO,    /* Text message to/from endpoint */
	GET,     /* Start file download */
	PUT,     /* Start file upload */
	RETR     /* Request packet retransmission */
} packettype;

/* transmission unit for this application's protocol */
typedef struct
{
	packettype  control;         /* type of packet */
	int         sequence;        /* packet's sequence number */
	int         loadsize;        /* number of bytes utilized in data */
	char        data[LOADSIZE];  /* payload of the packet */
	int         checksum;        /* error detection data */
} PACKET;

const char* getPacketType(const PACKET* packet);
void printPacket(const PACKET* packet);

