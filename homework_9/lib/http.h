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
#define BUFFER_SIZE 1000


class Request
{
public:
    Request();

private:
    std::string method;
    std::string path;
    std::string body;
    std::string version;
    std::map<std::string, std::string> headers;


    // read request from the socket and convert it into Request class object
    void get_request(int socket_fd);
    void parse(char * message, ssize_t received_bytes);

};

class Response
{
public:
    Response();

private:
    std::string method;
    std::string path;
    std::string body;
    std::string version;
    std::map<std::string, std::string> headers;

    // produce response for given request
    void produce_response(const Request& request);

    // send response to the client converting it from Response class object
    void send_response(int socket_fd) const;
};

/* name and return value of function ?? */
// main function for library users
void run();


#endif //HOMEWORK_9_HTTP_H
