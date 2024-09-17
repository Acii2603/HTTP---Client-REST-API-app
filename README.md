# HTTP Application - REST API Client

**Author**: Alexandru-Andrei Ionita  
**Year**: 2023-2024

## Description

This application is an HTTP client that allows the user to make requests to a library server. The client can create, delete, and view details about books. The server uses an authentication and authorization system:

- **Authentication**: Done via a session cookie.
- **Authorization**: Done via a JWT token obtained when entering the library.

## Implementation Details

### Authentication

To access the server's resources, the client must be authenticated. This is achieved by obtaining a session cookie during the login process.

### Authorization

To access the server's resources, the client must be authorized. This is done via a JWT token that is obtained when entering the library. The token is sent in the request headers to the server and is verified.

### Requests

Requests are made using the functions: `compute_get_request`, `compute_post_request`, and `compute_delete_request`. These functions take parameters such as the URL, payload, headers, and cookies, and return a string representing the HTTP request.

These requests are sent to the server using the `send_to_server` function, which receives the socket file descriptor and the request, then returns the response from the server.

### Responses

Responses are received from the server using the `receive_from_server` function, then parsed and displayed based on the current context. If a book or a list of books is received, they are displayed in JSON format with pretty-print formatting. Otherwise, the result of the request is displayed: success, failure (client-side), or internal server error.

### Error Handling

Errors are handled based on the current context or the status code received from the server. If a status code other than 200 is received, an error message is displayed.

## JSON Parsing

To work with JSON, the [Parson](https://github.com/kgabis/parson) library was used. This library was chosen because it is simple to use and well-documented. It's open-source and easy to integrate into the project.

## Compilation and Execution

To compile the project, run the command `make`.

To run the project, execute the command `./client`.

## Commands

### `register`

```bash
register
<username: String>
<password: String>
```

### `login`

```bash
login
<username: String>
<password: String>
```

### `enter_library`

```bash
enter_library
```

### `get_books`

```bash
get_books
```

### `get_book`

```bash
get_book
<id: Number>
```

### `add_book`

```bash
add_book
<title: String>
<author: String>
<genre: String>
<publisher: String>
<page_count: Number>
```

### `delete_book`

```bash
delete_book
<id: Number>
```

### `logout`

```bash
logout
```

### `exit`

```bash
exit
```

## Conclusion

In the end, this was a challenging but interesting project because I learned how to work with HTTP, JSON, JWT, and cookies. I also learned how to make requests to a server and parse the responses.
