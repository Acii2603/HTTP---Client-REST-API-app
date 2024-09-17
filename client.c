#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "client_commands.h"

// method to 

int main(int argc, char *argv[])
{
    char* JWT = NULL;
    char* session_cookie = NULL;

    char* keyboard_input = (char*)malloc(LINELEN * sizeof(char));

    while (1) {
        memset(keyboard_input, 0, LINELEN);
        fgets(keyboard_input, LINELEN - 1, stdin);
        keyboard_input[strlen(keyboard_input) - 1] = '\0';

        if (strcmp(keyboard_input, "register") == 0) {
            register_command(&session_cookie);
        } else if (strcmp(keyboard_input, "login") == 0) {
            login_command(&session_cookie);
        } else if (strcmp(keyboard_input, "enter_library") == 0) {
            enter_library_command(&session_cookie, &JWT);
        } else if (strcmp(keyboard_input, "get_books") == 0) {
            get_books_command(&session_cookie, &JWT);
        } else if (strcmp(keyboard_input, "get_book") == 0) {
            get_book_command(&session_cookie, &JWT);
        } else if (strcmp(keyboard_input, "add_book") == 0) {
            add_book_command(&session_cookie, &JWT);
        } else if (strcmp(keyboard_input, "delete_book") == 0) {
            delete_book_command(&session_cookie, &JWT);
        } else if (strcmp(keyboard_input, "logout") == 0) {
            logout_command(&session_cookie, &JWT, '1');
        } else if (strcmp(keyboard_input, "exit") == 0) {
            exit_command(&session_cookie, &JWT);
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    free(keyboard_input);

    return 0;
}
