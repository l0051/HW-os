#include "http.h"


Response::Response()
    : method{}
    , path{}
    , headers{}
    , body{}
    , version{}
{}

Request::Request()
    : method{}
    , path{}
    , headers{}
    , body{}
    , version{}
{}

void Request::parse(char * message, ssize_t received_bytes)
{
    if (method[method.size() - 1] != '\n')
    {
        for (int i = 0; i < received_bytes; ++i)
        {
            if (message[i] != ' ')
            {
                method.push_back(message[i]);
            }
            else
            {
                method.push_back('\n');
            }
        }
    }
    else if (path[path.size() - 1] != '\n')
    {
        for (int i = 0; i < received_bytes; ++i)
        {
            if (message[i] != ' ')
            {
                path.push_back(message[i]);
            }
            else
            {
                path.push_back('\n');
            }
        }
    }
    else if (version[version.size() - 1] != '\n')
    {
        for (int i = 0; i < received_bytes; ++i)
        {
            version.push_back(message[i]);
        }
    }
    else //headers and body
    {

    }
}

void Request::get_request(int socket_fd)
{
    ssize_t received_bytes = -1;

    /*
    // variables for parsing
    int space_count = 0;
    bool is_first_line_over = false;
    bool is_headers_over = false;
     */

    while (received_bytes != 0) {
        char * message = new char[BUFFER_SIZE];
        received_bytes = recv(socket_fd, (void *) message, BUFFER_SIZE, 0);

        if (received_bytes < 0) {
            std::cerr << "Could not read from client. Error: " << errno << std::endl;
            close(socket_fd);

            //return error Request (empty one)
            // change for not empty
            *this = Request{};
            return;
        }
        if (received_bytes == 0)
        {
            break;
        }

        //
        this->parse(message, received_bytes);
    }
}

void Response::produce_response(const Request& request)
{

}

void Response::send_response(int socket_fd) const
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
        Request request;
        request.get_request(client_socket_fd);
        Response response;
        response.produce_response(request);
        response.send_response(client_socket_fd);
         **/

    }
}