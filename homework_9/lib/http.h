#ifndef HOMEWORK_9_HTTP_H
#define HOMEWORK_9_HTTP_H


#include <iostream>
#include <string>
#include <map>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NUM_OF_THREADS 1000

class Response
{
public:
    //Response();

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

// read request from the socket and convert it into Request class object
Request get_request(int socket_fd);

// produce response for given request
Response produce_response(const Request& request);

// send response to the client converting it from Response class object
void send_response(const Response& response, int socket_fd);

/* name and return value of function ?? */
// main function for library users
void run();


#endif //HOMEWORK_9_HTTP_H
