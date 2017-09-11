# libcpnet

![language](https://img.shields.io/badge/language-c-blue.svg)
![c](https://img.shields.io/badge/std-c99-blue.svg)
![GCC](https://img.shields.io/badge/GCC-5.4.0-blue.svg)
![GCC](https://img.shields.io/badge/GCC-4.9.3-blue.svg)
![GCC](https://img.shields.io/badge/GCC-4.8.5-blue.svg)
![MSVC](https://img.shields.io/badge/MSVC-14-red.svg)
![license](https://img.shields.io/badge/license-GPLv3-blue.svg)
[![Build status](https://ci.appveyor.com/api/projects/status/aavivsw7msa57jgp?svg=true)](https://ci.appveyor.com/project/soroush/libcpnet)
[![Build Status](https://travis-ci.org/soroush/libcpnet.svg?branch=dev)](https://travis-ci.org/soroush/libcpnet)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/92107eda69f8404495ee2a4953c72d9b)](https://www.codacy.com/app/soroush/libcpnet?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=soroush/libcpnet&amp;utm_campaign=Badge_Grade)

A thin wrapper cross-platform networking API on socket level. This library is
a very simple thin wrapper around POSIX (Berkeley) socket implementation and 
its Windows bastardisation. The main target is to provide a cross-platform 
API to access UDP and TCP sockets on Windows and real Operating Systems.

## Usage

Connect, Send and Receive buffers in client side:

```c
/* Client-side sample code */
/* Initialize networking API (Only needed in Windows) */
net_init();
/* Start a client */
socket_t socket = net_socket(SOCK_STREAM);
/* Connect to port 50001 */
if(net_connect(socket, "127.0.0.1", 50001) != 0) 
{
    fprintf(stderr, "Unable to connect: %s", net_last_error();
    exit(-1);
}
char buffer[1024];
/* Write to socket */
ssizet io_size = net_write(socket, buffer, 1024);
if(io_size < 0)
{
    fprintf(stderr, "Unable to write: %s", net_last_error();
    exit(-1);
}
io_size = net_read(socket, buffer, 1024);
if(read_size <= 0) {
    fprintf(stderr, "Unable to read: %s", net_last_error();
    exit(-1);
}
/* ... */
/* Cleanup (Only needed in Windows) */
net_clean();
```

Bind, Listen, Accept, Read, and Write in server side:

```c
/* Initialize networking API (if any needed) */
net_init();
/* Start a server */
socket_t socket = net_socket(SOCK_STREAM);
/* Bind on port 50001 */
uint16_t port = 50001;
if(net_bind(socket, NULL, &port) != 0) {
    fprintf(stderr, "Unable to bind: %s", net_last_error());
    exit(-1);
}
/* Listen for incomming connections (backlog=10) */
net_listen(socket, 10);
/* Accept clients */
char address[46];
socket_t client = net_accept(socket, address, &port);
fprintf(stderr, "Client connected. Remote Address:`%s' Assigned port number: %d\n", address, port);
char buffer[1024];
ssize_t r = net_read(client, buffer, 1024);
if(r < 0) {
    fprintf(stderr, "Unable to read from socket: ", net_last_error());
    exit(-1);
}
if(r == 0) {
    printf("Remote client stopped.");
    net_close(client);
    exit(-1);
}
ssize_t w = net_write(client, buffer, r);
if(w <= 0) {
    fprintf(stderr, "Unable to write to socket: ", net_last_error());
    exit(-1);
}
```


