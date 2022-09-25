#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "http/request/HttpRequest.hpp"
#include "http/response/HttpResponse.hpp"

int s;

void recv_impl(void* buf, size_t buffer_length) {
    recv(s, buf, buffer_length, 0);
}

int main() {
    s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(8000)
    };

    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    
    connect(s, (struct sockaddr*) &address, sizeof(address));

    // abc

    http::request::HttpRequest req(http::spec::METHOD_GET, http::spec::VERSION_HTTP_11, "/");

    req.headers.set("User-Agent", "cpp-http 1.0 (in development)");
    req.headers.set("Connection", "close");

    std::string buf = req.get();

    std::cout << "=== Sending ===" << std::endl;
    std::cout << buf;

    send(s, buf.c_str(), buf.length(), 0);

    std::cout << "Sent ... recv" << std::endl;

    // char bufa[10240];
    // recv(s, &bufa, sizeof(bufa), 0);

    // std::cout << bufa;

    // xyz

    http::response::recv(recv_impl);
}

