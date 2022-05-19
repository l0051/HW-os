#ifndef HOMEWORK_9_HTTP_H
#define HOMEWORK_9_HTTP_H

#include <iostream>
#include <string>
#include <map>
#include <cerrno>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>


#define NUM_OF_THREADS 1000
#define BUFFER_SIZE 1000

class Http_Server
{
public:
    struct Handler;
    explicit Http_Server(int port);
    Http_Server();
private:

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

        // set data from given string to class members
        void parse(const std::string& request, ssize_t received_bytes);

    };

    class Response
    {
    public:
        Response();

    private:
        std::string status_code;
        std::string status_text;
        std::string body;
        std::string version;
        std::map<std::string, std::string> headers;

        // send response to the client converting it from Response class object to bytes
        void send_response(int socket_fd) const;
    };

    // map of handlers for certain path and method
    std::map<std::pair<std::string , std::string>, Handler*> path_method_to_handle;
    int port;
public:

    // main function for library users
    void run();

    // struct for function to produce response for given request
    struct Handler
    {
        virtual Response handle(const Request& request) = 0;
    };

    // function for adding handler for certain path and method
    void add_handler(const std::string& path, const std::string& method, Handler* handler);
};

#endif //HOMEWORK_9_HTTP_H
