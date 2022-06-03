#include <iostream>
#include "lib/http.h"

int main() {
    Http_Server server(8080);
    server.run();
    return 0;
}
