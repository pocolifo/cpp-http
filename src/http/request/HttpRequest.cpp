#include <sstream>

#include "HttpRequest.hpp"

using namespace http::request;


HttpRequest::HttpRequest(HttpMethod method, HttpVersion version, std::string resource)  {
    this->method = method;
    this->version = version;
    this->resource = resource;
}

std::string HttpRequest::get() {
    std::stringstream buf;

    buf << this->method << " " << this->resource << " " << this->version << "\r\n";

    for (HttpHeader header : this->headers) {
        buf << header.key << ": " << header.value << "\r\n";
    }

    buf << "\r\n" << this->body;

    return buf.str();
}