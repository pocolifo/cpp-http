#include "HttpHeaders.hpp"

using namespace http::spec;


void HttpHeaders::set(const std::string key, const std::string value) {
    for (HttpHeader header : this->headers) {
        if (header.key == key) {
            header.value = value;

            return;
        }
    }

    this->headers.push_back({key, value});
}

bool HttpHeaders::has(const std::string key) {
    for (HttpHeader header : this->headers) {
        if (header.key == key) return true;
    }

    return false;
}

void HttpHeaders::remove(const std::string key) {
    for (std::vector<HttpHeader>::iterator iter = this->headers.begin(); iter != this->headers.end(); iter++) {
        if ((*iter).key == key) this->headers.erase(iter);
    }
}

HttpHeader HttpHeaders::get(const std::string key) {
    for (HttpHeader header : this->headers) {
        if (header.key == key) return header;
    }
}

std::vector<http::spec::HttpHeader>::iterator HttpHeaders::begin() {
    return this->headers.begin();
}

std::vector<http::spec::HttpHeader>::iterator HttpHeaders::end() {
    return this->headers.end();
}
