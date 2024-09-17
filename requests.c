#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char **headers,
                            int headers_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s/%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // add host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add necessary headers
    compute_message(message, "Connection: keep-alive");

    // add optional headers
    if (headers_count != 0) {
        for (int i = 0; i < headers_count; i++) {
            sprintf(line, "%s", headers[i]);
            compute_message(message, line);
        }
    }

    if (cookies_count != 0) {
        // add cookies to the message if any
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }   
    }

    free(line);

    // add the final newline
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                            char **cookies, int cookies_count,
                            char **headers, int headers_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // add method and url
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add necessary headers - Content-Type and Content-Length are mandatory for POST requests
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    // calculate and add the Content-Length header
    int content_length = strlen(body_data);

    sprintf(line, "Content-Length: %d", content_length);
    compute_message(message, line);

    // add optional headers
    if (headers_count != 0) {
        for (int i = 0; i < headers_count; i++) {
            sprintf(line, "%s", headers[i]);
            compute_message(message, line);
        }
    }

    if (cookies_count != 0) {
        // add cookies to the message if any
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }   
    }

    // add the final new line after the headers
    compute_message(message, "");

    // add the actual payload data
    compute_message(message, body_data);

    // free the allocated memory
    free(line);
    return message;
}

char* compute_delete_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char **headers,
                            int headers_count) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "DELETE %s/%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    // add method and url
    compute_message(message, line);

    // add host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add necessary headers
    compute_message(message, "Connection: keep-alive");

    // add headers
    if (headers_count != 0) {
        for (int i = 0; i < headers_count; i++) {
            sprintf(line, "%s", headers[i]);
            compute_message(message, line);
        }
    }

    if (cookies_count != 0) {
        // add cookies to the message if any
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }   
    }
    free(line);
    
    // add the final newline
    compute_message(message, "");

    return message;
}
