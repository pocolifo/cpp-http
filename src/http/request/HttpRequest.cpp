#include "HttpRequest.hpp"

using namespace http::request;


HttpRequest::HttpRequest(HttpMethod method, HttpVersion version, std::string resource)  {
    this->method = method;
    this->version = version;
    this->resource = resource;
}

std::string HttpRequest::get() {
    std::string s;

    s.append(this->method).append(" ").append(this->resource).append(" ").append(this->version).append("\r\n");

    for (HttpHeader header : this->headers) {
        s.append(header.key).append(": ").append(header.value).append("\r\n");
    }

    s.append("\r\n").append(this->body);

    return s;
}