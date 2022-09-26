#pragma once

#include <string>

#include "../spec/HttpMethod.hpp"
#include "../spec/HttpVersion.hpp"
#include "../spec/HttpHeaders.hpp"

using namespace http::spec;


namespace http::response
{
    class HttpResponse {
        public:
            short statusCode = -1;
            std::string version;
            std::string statusText;
            HttpHeaders headers;
            std::string body;

            std::string get();
    };

    HttpResponse recv(int(*next_bytes_func)(void* buf, size_t buffer_length));
}
