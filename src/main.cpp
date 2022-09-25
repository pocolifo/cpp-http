#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "http/request/HttpRequest.hpp"
#include "http/response/HttpResponse.hpp"

int s;

void recv_impl(void* buf, size_t buffer_length) {
    recv(s, buf, buffer_length, 0);
}

void connect_socket() {
    s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(8000)
    };

    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(s, (struct sockaddr*) &address, sizeof(address));
}

http::request::HttpRequest get(std::string resource) {
    http::request::HttpRequest r(http::spec::METHOD_GET, http::spec::VERSION_HTTP_11, resource);

    r.headers.set("Connection", "keep-alive");
    r.headers.set("User-Agent", "cpp-http");
    r.headers.set("Host", "127.0.0.1:8000");
    r.headers.set("Accept", "*/*");

    return r;
}

int main() {
    connect_socket();

    // request #1
    http::request::HttpRequest indexReq = get("/");
    send(s, indexReq.get().c_str(), indexReq.get().length(), 0);
    // http::response::recv(recv_impl);

    // request #2
    http::request::HttpRequest makefileReq = get("/Makefile");
    // makefileReq.headers.set("Connection", "close");

    // std::cout << makefileReq.get();

    send(s, makefileReq.get().c_str(), makefileReq.get().length(), 0);
    http::response::recv(recv_impl);

    close(s);
}

