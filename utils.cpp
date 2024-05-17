#include "utils.h"
#include "buffer.h"

// Print an error message and exit the program -- Taken from lab 09
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Open a connection to the server -- Taken from lab 09
int open_connection(const char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    // Create a socket
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR - Opening socket");

    // Set the server details
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    // Connect the socket to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR - Connecting socket to server");

    return sockfd;
}

// Close the connection to the server -- Taken from lab 09
void close_connection(int sockfd)
{
    close(sockfd);
}

// Send a message to the server -- Taken from lab 09
void send_to_server(int sockfd, string message_string)
{
    const char *message = message_string.c_str();
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
            error("ERROR writing message to socket");

        if (bytes == 0)
            break;

        sent += bytes;
    } while (sent < total);
}

// Receive a message from the server -- Taken from lab 09
string receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do
    {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0)
        {
            error("ERROR reading response from socket");
        }

        if (bytes == 0)
        {
            break;
        }

        buffer_add(&buffer, response, (size_t)bytes);

        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0)
        {
            header_end += HEADER_TERMINATOR_SIZE;

            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);

            if (content_length_start < 0)
            {
                continue;
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t)header_end;

    while (buffer.size < total)
    {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0)
        {
            error("ERROR reading response from socket");
        }

        if (bytes == 0)
        {
            break;
        }

        buffer_add(&buffer, response, (size_t)bytes);
    }
    buffer_add(&buffer, "", 1);

    string response_string(buffer.data);
    return response_string;
}