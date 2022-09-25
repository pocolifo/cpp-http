#include <sstream>
#include <iostream> // temp

#include "HttpResponse.hpp"

#define BUFFER_SIZE 10

using namespace http::response;


HttpResponse http::response::recv(void(*next_bytes_func)(void* buf, size_t buffer_length)) {
    char buf[BUFFER_SIZE];
    next_bytes_func(&buf, BUFFER_SIZE);

    return HttpResponse("HTTP/1.1", 200, "OK");
}

HttpResponse::HttpResponse(HttpVersion version, short statusCode, std::string statusText) {
    this->version = version;
    this->statusCode = statusCode;
    this->statusText = statusText;
}

std::string HttpResponse::get() {
    std::stringstream buf;

    buf << this->version << " " << this->statusCode << " " << this->statusText << "\r\n";

    for (HttpHeader header : this->headers) {
        buf << header.key << ": " << header.value << "\r\n";
    }

    buf << "\r\n" << this->body;

    return buf.str();
}