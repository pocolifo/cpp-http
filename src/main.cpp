#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "http/request/HttpRequest.hpp"
#include "http/response/HttpResponse.hpp"

int s;

int recv_impl(void* buf, size_t buffer_length) {
    return recv(s, buf, buffer_length, 0);
}

void connect_socket(const char* host, int port) {
    s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    inet_pton(AF_INET, host, &address.sin_addr);

    connect(s, (struct sockaddr*) &address, sizeof(address));
}

http::request::HttpRequest get(std::string resource) {
    http::request::HttpRequest r(http::spec::METHOD_GET, http::spec::VERSION_HTTP_10, resource);

    r.headers.set("Connection", "close");
    r.headers.set("User-Agent", "custom cpp http client");
    r.headers.set("Accept", "*/*");

    return r;
}

int main(int argCount, char** arguments) {
    if (argCount != 4) {
        std::cerr << "usage: " << arguments[0] << " (ip address) (port) (http path)" << std::endl;
        return -1;
    }

    connect_socket(arguments[1], std::stoi(arguments[2]));
    
    http::request::HttpRequest request = get(arguments[3]);
    send(s, request.get().c_str(), request.get().length(), 0);
    http::response::HttpResponse response = http::response::recv(recv_impl);

    
    std::cout << "PROTOCOL    " << response.version << std::endl;
    std::cout << "STATUS CODE " << response.statusCode << std::endl;
    std::cout << "STATUS TEXT " << response.statusText << std::endl;

    for (HttpHeader header : response.headers) {
        std::cout << header.key << " :: " << header.value << std::endl;
    }

    /*
    std::cout << std::endl << "=== BODY START ===" << std::endl << response.body << std::endl << "=== BODY END ===" << std::endl;
    */
   
    close(s);
}

