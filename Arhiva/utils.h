#ifndef _UTILS_
#define _UTILS_

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <errno.h>

using namespace std;

/* JSON library */
#include "json.hpp"
using json = nlohmann::json;

/* Serve details */
#define SERVER_IP "34.246.184.49"
#define SERVER_PORT 8080
#define IP_PORT "34.246.184.49:8080"

/* Structure to hold the client details and all the information he needs */
typedef struct Client
{
    // Server details
    int socket;
    struct sockaddr_in server;

    // Client credentials
    string username = "";
    string password = "";

    // Client details
    bool logged_in = false;
    string JWT_token = "";
    vector<string> cookies;
} Client;

/* Function prototypes */
void error(const char *msg);
int open_connection(const char *host_ip, int portno, int ip_type, int socket_type, int flag);
void close_connection(int sockfd);
void send_to_server(int sockfd, string message_string);
string receive_from_server(int sockfd);

/* Constants definition */
#define NOT_FIND string::npos
#define BUFLEN 4096
#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)
#define HEADER_END_LINE "\r\n"

#endif