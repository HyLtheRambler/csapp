#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_URL_LENGTH 2488
#define NTHREADS 4
#define SBUFSIZE 16

struct line {
    char *url;
    int valid;
    int object_size;
    int used_count;
    char *buffer;
};
typedef struct line* set;
set cache;
int readcnt = 0;
sem_t mutex, w;

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

/* Global variables */
sbuf_t sbuf; /* Shared buf of connected descriptors */

void *thread(void *vargp);
int cached(char *url);
void forward_cache(char *url, int fd);
void cache_object(char *url, char *buffer, int bytes);
void doit(int fd);
void forward_requesthdrs(rio_t *rp, rio_t *wp);
int parse_uri(char *uri, char *hostname, char *path, char *port);
void get_filetype(char *filename, char *filetype);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

int main(int argc, char **argv) 
{
    int i, listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    Sem_init(&mutex, 0, 1);
    Sem_init(&w, 0, 1);

    /* Init proxy cache */
    cache = (struct line *) Malloc(10 * sizeof(struct line));
    for (i = 0; i < 10; ++i) {
        cache[i].url = (char *)Calloc(MAX_URL_LENGTH, sizeof(char));
        cache[i].valid = 0;
        cache[i].object_size = 0;
        cache[i].used_count = 0;
        cache[i].buffer = (char *)Calloc(MAX_OBJECT_SIZE, sizeof(char));
    }

    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }
    listenfd = Open_listenfd(argv[1]);

    sbuf_init(&sbuf, SBUFSIZE);
    for (i = 0; i < NTHREADS; i++) /* Create worker threads */
        Pthread_create(&tid, NULL, thread, NULL);

    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
    sbuf_insert(&sbuf, connfd);
    }
}
/* $end tinymain */

/*
 * thread - routine for handling a certain connfd
 */
/* $begin thread */
void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf);
        doit(connfd);
        Close(connfd);
    }
}
/* $end thread*/

/*
 * doit - handle one HTTP request/response transaction, 
 * check validity and and forward to target
 */
/* $begin doit */
void doit(int fd) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLINE], port[MAXLINE];
    rio_t rio_client, rio_server;
    int to_serverfd;
    int rc, n;

    /* For caching web objects */
    char buffer[MAX_OBJECT_SIZE];
    int bytes_accumulated = 0;

    /* Read request line and headers */
    Rio_readinitb(&rio_client, fd);
    if (!Rio_readlineb(&rio_client, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("Request from client:\r\n");
    printf("%s", buf);
    if ((rc = sscanf(buf, "%s %s %s", method, uri, version)) < 3) {
        printf("Received invalid request line");
        return;
    }
    if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "tiny proxy does not implement this method");
        return;
    }                                                    //line:netp:doit:endrequesterr

    /* Parse URI from GET request */
    if ((rc = parse_uri(uri, hostname, path, port) < 0)) {
        return;
    }



    /* if cached, directly serve the client and return */
    if (cached(uri)) {

        while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
            Rio_readlineb(&rio_client, buf, MAXLINE);
            printf("%s", buf);
        }

        printf("cached object\n");
        forward_cache(uri, fd);
        return;
    }

    if ((to_serverfd = Open_clientfd(hostname, port)) < 0) {
        return;
    }
    Rio_readinitb(&rio_server, to_serverfd);
    sprintf(buf, "GET %s HTTP/1.0\r\n", path);
    Rio_writen(to_serverfd, buf, strlen(buf));
    sprintf(buf, "Host: %s\r\n", hostname);
    Rio_writen(to_serverfd, buf, strlen(buf));
    sprintf(buf, user_agent_hdr);
    Rio_writen(to_serverfd, buf, strlen(buf));
    sprintf(buf, "Connection: close\r\n");
    Rio_writen(to_serverfd, buf, strlen(buf));
    sprintf(buf, "Proxy-Connection: close\r\n");
    Rio_writen(to_serverfd, buf, strlen(buf));
    forward_requesthdrs(&rio_client, &rio_server);                              //line:netp:doit:readrequesthdrs

    while((n = Rio_readnb(&rio_server, buf, MAXLINE)) > 0) {
        Rio_writen(fd, buf, n);
        if ((n + bytes_accumulated) < MAX_OBJECT_SIZE) {
            memcpy(buffer + bytes_accumulated, buf, n);
            bytes_accumulated += n;
        } else {
            bytes_accumulated += n;
        }
    }

    if (bytes_accumulated < MAX_OBJECT_SIZE) {
        cache_object(uri, buffer, bytes_accumulated);
    }

    Close(to_serverfd);
    
}
/* $end doit */

/*
 * cached - check if the item asked is cached
 * return 1 if cached, 0 otherwise
 */
/* $begin cached */
int cached(char *url)
{
    int i = 0;

    P(&mutex);
    readcnt++;
    if (readcnt == 1)
        P(&w);
    V(&mutex);

    for (; i < 10; ++i) {
        if (cache[i].valid && (!strcmp(cache[i].url, url))) {

            P(&mutex);
            readcnt--;
            if (readcnt == 0)
                V(&w);
            V(&mutex);

            return 1;
        }
    }

    P(&mutex);
    readcnt--;
    if (readcnt == 0)
        V(&w);
    V(&mutex);

    return 0;
}
/* $end cached */

/* 
 * forward_cached - directly read cached item and forward back to client
 */
/* $begin forward_cached */
void forward_cache(char *url, int fd)
{
    int i = 0;

    printf("Control reached here: 2\n");

    P(&mutex);
    readcnt++;
    if (readcnt == 1)
        P(&w);
    V(&mutex);

    for (; i < 10; ++i) {
        if (cache[i].valid && (!strcmp(cache[i].url, url))) {

            cache[i].used_count++;
            printf("Control reached here: 1\n");
            Rio_writen(fd, cache[i].buffer, cache[i].object_size);
            printf("Control reached here: 0\n");

            P(&mutex);
            readcnt--;
            if (readcnt == 0)
                V(&w);
            V(&mutex);

            return;
        }
    }

    P(&mutex);
    readcnt++;
    if (readcnt == 0)
        V(&w);
    V(&mutex);

}
/* $end forward_cached */

/*
 * cache_object - cache a new object. Applying LRU eviction policy
 */
/* $begin cache_object */
void cache_object(char *url, char *buffer, int bytes)
{
    printf("Cache_object called\n");
    P(&w);

    int i = 0;
    int least_used_index = 0;
    int least_used_count = 0x7fffffff;
    for (; i < 10; ++i) {
        if (cache[i].valid == 0) {
            printf("Cache_object here\n");
            cache[i].valid = 1;
            cache[i].object_size = bytes;
            strcpy(cache[i].url, url);
            memcpy(cache[i].buffer, buffer, bytes);
            cache[i].used_count = 0;

            V(&w);

            return;
        } else if (cache[i].used_count < least_used_count) {
            least_used_index = i;
            least_used_count = cache[i].used_count;
        }
    }

    memset(cache[least_used_index].url, 0, MAX_URL_LENGTH);
    strcpy(cache[least_used_index].url, url);
    cache[i].object_size = bytes;
    memcpy(cache[i].buffer, buffer, bytes);

    V(&w);
}
/* $end cache_object*/

/*
 * forward_requesthdrs - read and forward HTTP request headers from client
 * to server
 */
/* $begin forward_requesthdrs */
void forward_requesthdrs(rio_t *rp, rio_t *wp) 
{
    char buf[MAXLINE];
    ssize_t n;

    n = Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    Rio_writen(wp->rio_fd, buf, n);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
        n = Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
        Rio_writen(wp->rio_fd, buf, n);
    }
    return;
}
/* $end forward_requesthdrs */

/*
 * parse_uri - parse the request into at least the following fields:
 * the hostname, www.cmu.edu .e.g; the optional port;
 * and the path or query and everything following it;
 * $begin parse_uri 
 */
int parse_uri(char *uri, char *hostname, char *path, char *port) 
{
    char *hostname_start = strstr(uri, "://");
    char *path_start;
    char *port_start;
    int hostname_len, path_len, port_len;
    int uri_len = strlen(uri);
    if (!hostname_start) {
        printf("invalid url\n");
        return -1;
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
/* $end parse_uri */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Proxy Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web Proxy</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */