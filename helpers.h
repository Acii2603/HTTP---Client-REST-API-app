#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000

#define HOST "34.246.184.49"
#define PORT 8080

#define POSITIVE_RESPONSE 2
#define CLIENT_ERROR 4
#define INTERNAL_SERVER_ERROR 5

// returns the payload of a response containing books
char* get_payload_books(char* response);

// returns the payload of a response containing a book
char* get_payload_book(char* response);

// creates an authentification header for a token
char* compute_auth_header(char* token);

// checks if a string is a number
char is_number(char *s);

// returns a string containing a jwt for library access
char* get_jwt(char* response);

// returns a string containing a session cookie
char* get_cookie(char* response);

// returns code prefix
int get_code_prefix(int status_code);

// returns status code
int get_status_code(char *response);

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
