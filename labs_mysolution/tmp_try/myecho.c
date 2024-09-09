#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 8192

int parse_uri(char *uri, char *hostname, char *path, char *port);

int main(int argc, char **argv) {
    char uri[MAXLINE], hostname[MAXLINE], path[MAXLINE], port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <URL>\n", argv[0]);
        exit(0);
    }

    sscanf(argv[1], "%s", uri);
    parse_uri(uri, hostname, path, port);
    printf("Host: %s, Port: %s, Path: %s \n", hostname, port, path);
    return 0;
}

int parse_uri(char *uri, char *hostname, char *path, char *port) 
{
    char *hostname_start = strstr(uri, "://");
    char *path_start;
    char *port_start;
    int hostname_len, path_len, port_len;
    int uri_len = strlen(uri);
    if (!hostname_start) {
        printf("invalid url\n");
        exit(0);
    }
    hostname_start += 3;
    path_start = strchr(hostname_start, '/');
    port_start = strchr(hostname_start, ':');
    
    if (!path_start) {
        path_start = uri + uri_len;
    }
    if (!port_start) {
        port_start = path_start;
    }
    hostname_len = port_start - hostname_start;
    port_len = path_start - port_start;
    path_len = uri + uri_len - path_start;
    strncpy(hostname, hostname_start, hostname_len);
    hostname[hostname_len] = '\0';
    strncpy(path, path_start, path_len);
    path[path_len] = '\0';

    if (path[0] == '\0') {
        path[0] = '/';
        path[1] = '\0';
    }
    if (port_len == 0) {
        port[0] = '8';
        port[1] = '0';
        port[2] = '\0';
    } else {
        port_len--;
        port_start++;
        strncpy(port, port_start, port_len);
        port[port_len] = '\0';
    }
    return 0;
}