#ifndef HOMEWORK_9_HTTP_H
#define HOMEWORK_9_HTTP_H
#include <iostream>


class Response
{
public:
    Response();

private:
    std::string method;
    std::string path;


};

class Request
{
public:
    Request();

private:
    std::string method;
    std::string path;
};


Response produce_response(const Request& request);

#endif //HOMEWORK_9_HTTP_H
