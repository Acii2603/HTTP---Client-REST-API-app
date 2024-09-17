#include "client_commands.h"

void register_command(char** session_cookie) {
    // check if logged in
    if (*session_cookie != NULL) {
        printf("N/A | ERROR | Already logged in\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    // get username and password
    char* username = (char*)malloc(LINELEN * sizeof(char));
    char* password = (char*)malloc(LINELEN * sizeof(char));

    printf("username=");
    fgets(username, LINELEN - 1, stdin);

    printf("password=");
    fgets(password, LINELEN - 1, stdin);

    // remove newline characters
    username[strlen(username) - 1] = '\0';
    password[strlen(password) - 1] = '\0';

    // create body_data - JSON object
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char* body_data = json_serialize_to_string_pretty(root_value);

    // compute POST request to send
    char* message = compute_post_request(HOST, "/api/v1/tema/auth/register", "application/json", body_data, NULL, 0, NULL, 0);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Registered\n", status_code);
    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | User exists\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(username);
    free(password);
    free(body_data);
    free(message);
    free(response);
    json_value_free(root_value);
}

void login_command(char** session_cookie) {
    // check if logged in
    if (*session_cookie != NULL) {
        printf("N/A | ERROR | Already logged in\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    // get username and password
    char* username = (char*)malloc(LINELEN * sizeof(char));
    char* password = (char*)malloc(LINELEN * sizeof(char));

    printf("username=");
    fgets(username, LINELEN - 1, stdin);

    printf("password=");
    fgets(password, LINELEN - 1, stdin);

    // remove newline characters
    username[strlen(username) - 1] = '\0';
    password[strlen(password) - 1] = '\0';

    // create body_data - JSON object
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char* body_data = json_serialize_to_string_pretty(root_value);

    // compute POST request to send
    char* message = compute_post_request(HOST, "/api/v1/tema/auth/login", "application/json", body_data, NULL, 0, NULL, 0);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Logged in\n", status_code);
        *session_cookie = get_cookie(response);

        if (*session_cookie == NULL) {
            printf("N/A | ERROR | Invalid cookie received\n");
        }

    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid credentials\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(username);
    free(password);
    free(body_data);
    free(message);
    free(response);
    json_value_free(root_value);
}

void enter_library_command(char** session_cookie, char** JWT) {
    // check if logged in
    if (*session_cookie == NULL) {
        printf("N/A | ERROR | Not logged in\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // compute GET request to send
    char* message = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, cookies, 1, NULL, 0);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Entered library\n", status_code);
        *JWT = get_jwt(response);

        if (*JWT == NULL) {
            printf("N/A | ERROR | Invalid JWT received\n");
        }

    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid cookie\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(message);
    free(response);
    free(cookies);
    free(cookie);
}

void get_books_command(char** session_cookie, char** JWT) {
    // check if logged in and entered library
    if (*session_cookie == NULL || *JWT == NULL) {
        printf("N/A | ERROR | Not logged in or entered library\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // create auth_header
    char* auth_header = compute_auth_header(*JWT);
    // create headers
    char** headers = (char**)malloc(1 * sizeof(char*));
    headers[0] = auth_header;

    // compute GET request to send
    char* message = compute_get_request(HOST, "/api/v1/tema/library/books", NULL, cookies, 1, headers, 1);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Got books\n", status_code);
        char* payload = get_payload_books(response);
        // parse payload as JSON
        JSON_Value* root_value = json_parse_string(payload);
        JSON_Array* books = json_value_get_array(root_value);

        for (int i = 0; i < json_array_get_count(books); i++) {
            JSON_Object* book = json_array_get_object(books, i);
            JSON_Value* wrapping_value = json_object_get_wrapping_value(book);
            char* serialized = json_serialize_to_string_pretty(wrapping_value);
            printf("%s\n", serialized);
            
            free(serialized);
        }

        json_value_free(root_value);

    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid JWT\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(message);
    free(response);
    free(cookies);
    free(cookie);
    free(headers);
    free(auth_header);   
}

void get_book_command(char** session_cookie, char** JWT) {
    // check if logged in and entered library
    if (*session_cookie == NULL || *JWT == NULL) {
        printf("N/A | ERROR | Not logged in or entered library\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // create auth_header
    char* auth_header = compute_auth_header(*JWT);
    // create headers
    char** headers = (char**)malloc(1 * sizeof(char*));
    headers[0] = auth_header;

    // get book id
    char* book_id = (char*)malloc(LINELEN * sizeof(char));

    printf("id=");
    fgets(book_id, LINELEN - 1, stdin);

    // remove newline characters
    book_id[strlen(book_id) - 1] = '\0';

    // validate book_id
    if (is_number(book_id) == '0') {
        printf("N/A | ERROR | Invalid book id format\n");

        free(book_id);
        free(cookie);
        free(headers);
        free(auth_header);
        free(cookies);
        return;
    }

    // compute GET request to send
    char* message = compute_get_request(HOST, "/api/v1/tema/library/books", book_id, cookies, 1, headers, 1);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Got book\n", status_code);
        char* payload = get_payload_book(response);
        // parse payload as JSON
        JSON_Value* root_value = json_parse_string(payload);
        char* serialized = json_serialize_to_string_pretty(root_value);
        printf("%s\n", serialized);

        free(serialized);
        json_value_free(root_value);

    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid JWT\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(book_id);
    free(message);
    free(response);
    free(cookies);
    free(cookie);
    free(headers);
    free(auth_header);
}

void add_book_command(char** session_cookie, char** JWT) {
    // check if logged in and entered library
    if (*session_cookie == NULL || *JWT == NULL) {
        printf("N/A | ERROR | Not logged in or entered library\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // create auth_header
    char* auth_header = compute_auth_header(*JWT);
    // create headers
    char** headers = (char**)malloc(1 * sizeof(char*));
    headers[0] = auth_header;

    // get book details
    char* title = (char*)malloc(LINELEN * sizeof(char));
    char* author = (char*)malloc(LINELEN * sizeof(char));
    char* genre = (char*)malloc(LINELEN * sizeof(char));
    char* publisher = (char*)malloc(LINELEN * sizeof(char));
    char* page_count = (char*)malloc(LINELEN * sizeof(char));

    printf("title=");
    fgets(title, LINELEN - 1, stdin);

    printf("author=");
    fgets(author, LINELEN - 1, stdin);

    printf("genre=");
    fgets(genre, LINELEN - 1, stdin);

    printf("publisher=");
    fgets(publisher, LINELEN - 1, stdin);

    printf("page_count=");
    fgets(page_count, LINELEN - 1, stdin);

    // remove newline characters
    title[strlen(title) - 1] = '\0';
    author[strlen(author) - 1] = '\0';
    genre[strlen(genre) - 1] = '\0';
    publisher[strlen(publisher) - 1] = '\0';
    page_count[strlen(page_count) - 1] = '\0';

    // validate page_count
    if (is_number(page_count) == '0') {
        printf("N/A | ERROR | Invalid page count format\n");

        free(title);
        free(author);
        free(genre);
        free(publisher);
        free(page_count);
        free(cookie);
        free(headers);
        free(auth_header);
        free(cookies);
        return;
    }

    // create body_data - JSON object
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_string(root_object, "publisher", publisher);
    
    int page_count_int = atoi(page_count);
    json_object_set_number(root_object, "page_count", page_count_int);

    char* body_data = json_serialize_to_string_pretty(root_value);

    // compute POST request to send
    char* message = compute_post_request(HOST, "/api/v1/tema/library/books", "application/json", body_data, cookies, 1, headers, 1);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Added book\n", status_code);
    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid JWT\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(title);
    free(author);
    free(genre);
    free(publisher);
    free(page_count);
    free(body_data);
    free(message);
    free(response);
    free(cookies);
    free(cookie);
    free(headers);
    free(auth_header);
    json_value_free(root_value);
}

void delete_book_command(char** session_cookie, char** JWT) {
    // check if logged in and entered library
    if (*session_cookie == NULL || *JWT == NULL) {
        printf("N/A | ERROR | Not logged in or entered library\n");
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // create auth_header
    char* auth_header = compute_auth_header(*JWT);
    // create headers
    char** headers = (char**)malloc(1 * sizeof(char*));
    headers[0] = auth_header;

    // get book id
    char* book_id = (char*)malloc(LINELEN * sizeof(char));

    printf("id=");
    fgets(book_id, LINELEN - 1, stdin);

    // remove newline characters
    book_id[strlen(book_id) - 1] = '\0';
    
    // validate book_id
    if (is_number(book_id) == '0') {
        printf("N/A | ERROR | Invalid book id format\n");

        free(book_id);
        free(cookie);
        free(headers);
        free(auth_header);
        free(cookies);
        return;
    }

    // compute DELETE request to send
    char* message = compute_delete_request(HOST, "/api/v1/tema/library/books", book_id, cookies, 1, headers, 1);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        printf("%d | SUCCESS | Deleted book\n", status_code);
    } else if (prefix == CLIENT_ERROR) {
        printf("%d | ERROR | Invalid JWT\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(book_id);
    free(message);
    free(response);
    free(cookies);
    free(cookie);
    free(headers);
    free(auth_header);
}

void logout_command(char** session_cookie, char** JWT, char print_message) {
    // check if logged in
    if (*session_cookie == NULL && print_message == '1') {
        printf("N/A | ERROR | Not logged in\n");
        return;
    } else if (print_message == '0' && *session_cookie == NULL) {
        return;
    }

    // open_connection
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char** cookies = (char**)malloc(1 * sizeof(char*));
    // add connect.sid cookie
    char* cookie = (char*)malloc(strlen(*session_cookie) + strlen("connect.sid=") + 1);
    strcpy(cookie, "connect.sid=");
    strcat(cookie, *session_cookie);
    cookies[0] = cookie;

    // compute GET request to send
    char* message = compute_get_request(HOST, "/api/v1/tema/auth/logout", NULL, cookies, 1, NULL, 0);

    // send_to_server
    send_to_server(sockfd, message);

    // receive_from_server
    char* response = receive_from_server(sockfd);

    // parse response
    int status_code = get_status_code(response);
    int prefix = get_code_prefix(status_code);

    if (prefix == POSITIVE_RESPONSE) {
        if (print_message == '1')
            printf("%d | SUCCESS | Logged out\n", status_code);

        // free session_cookie and JWT
        if (*JWT != NULL) {
            free(*JWT);
        }
        *JWT = NULL;

        if (*session_cookie != NULL) {
            free(*session_cookie);
        }
        *session_cookie = NULL;

    } else if (prefix == CLIENT_ERROR) {
        if (print_message == '1')
            printf("%d | ERROR | Invalid cookie\n", status_code);
    } else if (prefix == INTERNAL_SERVER_ERROR) {
        if (print_message == '1')
            printf("%d | OOF | Internal server error\n", status_code);
    }

    // close_connection
    close_connection(sockfd);

    free(message);
    free(response);
    free(cookies);
    free(cookie);
}

void exit_command(char** session_cookie, char** JWT) {
    // logout and exit program
    logout_command(session_cookie, JWT, '0');
    return;
}
