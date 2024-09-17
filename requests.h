#ifndef _REQUESTS_
#define _REQUESTS_

// returns a string containing a get request
char *compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count, char **headers,
                            int headers_count);

// returns a string containing a post request
char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
							char** cookies, int cookies_count,
							char **headers, int headers_count);

// returns a string containing a delete request
char *compute_delete_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count, char **headers,
                            int headers_count);

#endif
