#ifndef HOMEWORK_9_HTTP_H
#define HOMEWORK_9_HTTP_H


#include <iostream>
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NUM_OF_THREADS 1000

class Response
{
public:
    Response();

private:
    std::string method;
    std::string path;
    std::string body;
    std::map<std::string, std::string> headers;
};

class Request
{
public:
    //Request();

private:
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
};

// read request from the socket
Request get_request(int socket_fd);

// produce response for given request
Response produce_response(const Request& request);

// send response to the client
void send_response(const Response& response, int socket_fd);

/* name and return value of function ?? */
// main function for library users
void _server_(int socket_fd);


#endif //HOMEWORK_9_HTTP_H
