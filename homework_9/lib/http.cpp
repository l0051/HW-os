#include "http.h"

//Http::Response() = default;

//Http::Request() = default;

Request get_request(int socket_fd)
{

}

Response produce_response(const Request& request)
{

}

void send_response(const Response& response, int socket_fd)
{

}

void run()
{

    int port = 8080;

    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket_fd < 0){
        std::cerr << "Error while creating a socket" << std::endl;
        exit(errno);
    }

    struct sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(port); // port?
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int bound = bind(server_socket_fd, (const struct sockaddr *) &address, sizeof(address));

    if(bound < 0){
        std::cerr << "Could not bind to given port" << std::endl;
        exit(errno);
    }

    int listening = listen(server_socket_fd, NUM_OF_THREADS);

    if(listening < 0){
        std::cerr << "Could not start listening" << std::endl;
        exit(errno);
    }

    //thread pool .. ?creating

    while (true) {
        struct sockaddr_in client_address{};
        unsigned int client_address_length;
        int client_socket_fd = accept(server_socket_fd,(struct sockaddr*)  &client_address, &client_address_length);

        /// stex piti kcenq functian thread pool

        /// es thread-i argument functiai mej
        /**
        Request request = get_request(client_socket_fd);
        Response response = produce_response(request);
        send_response(response, client_socket_fd);
         **/

    }
}