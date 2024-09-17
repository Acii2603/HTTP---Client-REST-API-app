#ifndef CLIENT_COMMANDS_H
#define CLIENT_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "requests.h"
#include "helpers.h"
#include "parson.h"

void register_command(char** session_cookie);
void login_command(char** session_cookie);
void enter_library_command(char** session_cookie, char** JWT);
void get_books_command(char** session_cookie, char** JWT);
void get_book_command(char** session_cookie, char** JWT);
void add_book_command(char** session_cookie, char** JWT);
void delete_book_command(char** session_cookie, char** JWT);
void logout_command(char** session_cookie, char** JWT, char flag);
void exit_command(char** session_cookie, char** JWT);

#endif
