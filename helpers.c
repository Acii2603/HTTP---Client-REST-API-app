#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "buffer.h"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}

// returns code prefix
int get_code_prefix(int status_code) {
    return status_code / 100;
}

// returns status code
int get_status_code(char *response) {
    char *status_code = strstr(response, "HTTP/1.1 ");
    if (status_code == NULL) {
        return -1;
    }
    status_code += 9;
    return atoi(status_code);
}

// checks if a string is a number
char is_number(char *s) {
    for (int i = 0; i < strlen(s); i++) {
        if (!isdigit(s[i])) {
            return '0';
        }
    }
    return '1';

}

// returns a string containing a jwt for library access
char* get_jwt(char* response) {

    // format: {"token":"<jwt>"}
    char* token = strstr(response, "\"token\":\"");
    if (token == NULL) {
        return NULL;
    }
    token += 9;
    char* end = strstr(token, "\"");
    if (end == NULL) {
        return NULL;
    }
    char* jwt = (char*)calloc(end - token + 1, sizeof(char));
    strncpy(jwt, token, end - token);
    return jwt;
}

// returns a string containing a session cookie
char* get_cookie(char* response) {
    char* cookie = strstr(response, "connect.sid");
    if (cookie == NULL) {
        return NULL;
    }
    cookie += 12;
    char* end = strstr(cookie, ";");
    if (end == NULL) {
        return NULL;
    }
    char* session_cookie = (char*)calloc(end - cookie + 1, sizeof(char));
    strncpy(session_cookie, cookie, end - cookie);
    return session_cookie;
}

// creates an authentification header for a token
char* compute_auth_header(char* token) {
    char* auth_header = (char*)calloc(strlen(token) + strlen("Authorization: Bearer ") + 1, sizeof(char));
    strcpy(auth_header, "Authorization: Bearer ");
    strcat(auth_header, token);
    return auth_header;
}

// returns the payload of a response (JSON array)
char* get_payload_books(char* response) {
    char* payload = strstr(response, "[");
    if (payload == NULL) {
        return NULL;
    }
    return payload;
}

// returns the payload of a response (JSON object)
char* get_payload_book(char* response) {
    char* payload = strstr(response, "{");
    if (payload == NULL) {
        return NULL;
    }
    return payload;
}


