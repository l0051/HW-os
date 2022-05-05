#include "http.h"

//Http::Response() = default;

//Http::Request() = default;

Request get_request(int socket_fd) // arg?
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port); // port?
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int bound = bind(server_fd, (const struct sockaddr*) &address, size_of(address));

    int listening = listen(socket_fd, NUM_OF_THREADS);

    //thread pool ..
}

Response produce_response(const Request& request)
{

}

void send_response(const Response& response, int socket_fd)
{

}

void _server_(int socket_fd)
{
    while (true)
    {
        get_request(socket_fd);
    }
}
