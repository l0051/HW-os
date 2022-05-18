#include "http.h"


Response::Response()
    : status_code{}
    , status_text{}
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

// ! parse for body and headers
void Request::parse(const std::string& request, ssize_t received_bytes)
{
    int index = 0;
    while (method[method.size() - 1] != ' ' && index < received_bytes)
    {
        method.push_back(request[index]);
        ++index;
    }
    while (path[path.size() - 1] != ' ' && index < received_bytes)
    {
        path.push_back(request[index]);
        ++index;
    }
    while (version[version.size() - 1] != '\n' && index < received_bytes)
    {
        version.push_back(request[index]);
        ++index;
    }
    while (index < received_bytes)
    {
        std::string key;
        std::string value;
        if (request[index] == '\n')
        {
            ++index;
            break;
        }
        while (index < received_bytes && request[index] != ':')
        {
                key.push_back(request[index]);
                ++index;
        }
        while (index < received_bytes && request[index] != '\n')
        {
                value.push_back(request[index]);
                ++index;
        }
        headers.insert({key, value});
    }
    while (index < received_bytes)
    {
        auto it = headers.find("Content-Length");
        if (it != headers.end())
            break;
        int cont_length = atoi((it->second).c_str());
        while (body.size() < cont_length && index < received_bytes)
        {
           body.push_back(request[index]);
           ++index;
        }
    }
}

// done.
void Request::get_request(int socket_fd)
{
    ssize_t received_bytes = -1;
    std::string request;
    while (received_bytes != 0) {
        char* message = new char[BUFFER_SIZE];
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
        request += message;
    }

    this->parse(request, received_bytes);
}

void Response::produce_response(const Request& request)
{

}

void Response::send_response(int socket_fd) const
{
    std::string response;
    // make string response
    response += version + ' ';
    response += status_code + ' ';
    response += status_text + '\n';
    // response += headers

    char * ch_response = const_cast<char *>(response.c_str());
    ssize_t sent_bytes = send(socket_fd, (void *) ch_response, response.size(), 0);

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
    address.sin_port = htons(port);
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

    //thread pool ..

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